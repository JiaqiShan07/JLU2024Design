#include "all_h_files.h"
#include "user_system.h"
// ���ļ����ذ�������
int loadPackagesFromFile(PackageSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return 0;
    }

    // ��ȡ��������
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // �����ȡ�������ݲ���������
    PackageNode temp;
    system->head = NULL;
    system->package_count = 0;

    for (int i = 0; i < count; i++) {
        if (fread(&temp, sizeof(PackageNode), 1, file) != 1) {
            fclose(file);
            return 0;
        }

        PackageNode* new_node = (PackageNode*)malloc(sizeof(PackageNode));
        if (new_node == NULL) {
            fclose(file);
            return 0;
        }

        *new_node = temp;
        new_node->next = NULL;

        // �������Ƿ񳬹�����δȡ��
        if (new_node->status == PENDING_PICKUP) {
            time_t current_time = time(NULL);
            double time_diff = difftime(current_time, new_node->store_time);
            if (time_diff > STRANDED_TIME) {
                new_node->status = STRANDED;
                new_node->stranded_time= (int)(time_diff- STRANDED_TIME)/ ONE_DAY;
                //����ʱ��Ҫ��ȥ�����Ļ�׼ʱ��
            }
        }
        // ���½ڵ���ӵ�����ĩβ
        if (system->head == NULL) {
            system->head = new_node;
        } else {
            PackageNode* current = system->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
        system->package_count++;
    }

    fclose(file);
    return 1;
}

// ����������ݵ��ļ�
int savePackagesToFile(PackageSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "wt");
    if (file == NULL) {
        return 0;
    }

    // д���������
    if (fwrite(&system->package_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // ���д���������
    PackageNode* current = system->head;
    while (current != NULL) {
        if (fwrite(current, sizeof(PackageNode), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }

    fclose(file);
    return 1;
}

// ��ʼ����������ϵͳ
PackageSystem* initPackageSystem() {
    PackageSystem* system = (PackageSystem*)malloc(sizeof(PackageSystem));
    if (system == NULL) {
        printf("�ڴ����ʧ��\n");
        return NULL;
    }
    system->head = NULL;
    system->package_count = 0;
    return system;
}

// ����°���
int addPackage(PackageSystem* system,
               const char* username,
               const char* description,
               PackageSize size,
               float weight_kg,
               PackageUrgency urgency,
               PackageType type,
               PackageStorage storage,
               PackageStatus status) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return 0;
    }

    PackageNode* new_node = (PackageNode*)malloc(sizeof(PackageNode));
    if (new_node == NULL) {
        printf("�ڴ����ʧ��\n");
        return 0;
    }

    // ʹ��ר�����������ɰ���ID
    int new_id = generatePackageId(system);
    if (new_id == 0) {
        free(new_node);
        return 0;
    }
    //��ʼ������������
    new_node->package_id = new_id;
    strncpy(new_node->username, username, MAX_USERNAME_LENGTH - 1);
    new_node->username[MAX_USERNAME_LENGTH - 1] = '\0';
    strncpy(new_node->description, description, MAX_NAME_LENGTH - 1);
    new_node->description[MAX_NAME_LENGTH - 1] = '\0';
    new_node->status = status;
    new_node->size = size;
    new_node->weight_kg = weight_kg;
    new_node->urgency = urgency;
    new_node->type = type;
    new_node->storage = storage;
    new_node->stranded_time= 0;
    new_node->shelf_number = -1;
    new_node->layer_number = -1;
    new_node->pickup_name[0] = '\0';
    // ʹ�ð���λ��������������ܺźͲ��
    int shelf_number, layer_number;
    if (!generatePackageLocation(system, &shelf_number, &layer_number)) {
        free(new_node);
        return 0;
    }
    new_node->shelf_number = shelf_number;
    new_node->layer_number = layer_number;

    // ����ȡ����
    snprintf(new_node->pickup_code, MAX_PICKUP_CODE_LENGTH, "%d-%d-%d",
             new_node->shelf_number, new_node->layer_number,
             new_node->package_id);

    // ��¼�洢ʱ��
    new_node->store_time = time(NULL);
    new_node->pickup_time = 0;  // ��ʼ��ȡ��ʱ��Ϊ0

    new_node->next = NULL;

    // ���½ڵ���ӵ�����ĩβ
    if (system->head == NULL) {
        system->head = new_node;
    } else {
        PackageNode* current = system->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    system->package_count++;

    // ��Ӱ������Զ����浽�ļ�
    if (!savePackagesToFile(system, PACKAGE_FILE)) {
        printf("�����������ʧ��\n");
    }

    return new_node->package_id;
}

// �����û�����ѯ����
void queryPackagesByUsername(PackageSystem* system, const char* username) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    int found = 0;
    printf("----------------------------------------\n");
    printf("�û� %s �İ�����Ϣ:\n", username);
    PackageNode* current = system->head;
    printf("����ID\t\tȡ����\t\t\t״̬\t\t������Ϣ\n");
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (current->status == PENDING_PICKUP||current->status == STRANDED) {
                printf("%d\t\t%s\t\t%s\t\t[%d]\n", current->package_id,
                       current->pickup_code,
                       packageSatatusToString(current->status),
                       current->package_id);
            } else {
                printf("%d\t\t%s\t\t\t%s\t\t[%d]\n", current->package_id,
                       "- - -",

                       packageSatatusToString(current->status),
                       current->package_id);
            }

            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("----------------------------------------\n");
        printf("δ�ҵ����û��İ���\n");
        printf("----------------------------------------\n");
        return;
    }
    printf("----------------------------------------\n");
    printf("�������ID�Բ鿴������ϸ��Ϣ������0��������");
    int package_id;
    package_id = getValidatedIntegerInput(1000, 9999, 1);
    if (package_id != 0) {
        current = system->head;
        while (current != NULL) {
            if (current->package_id == package_id &&
                strcmp(current->username, username) == 0) {
                displayPackageDetails(current);
                break;
            }
            current = current->next;
        }
        if (current == NULL) {
            printf("δ�ҵ�����\n");
        }
    }
}

// ȡ������
int pickupPackage(PackageSystem* system, int package_id) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return 0;
    }

    PackageNode* current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            if (current->status == PENDING_PICKUP) {
                current->status = PICKED_UP;
                current->pickup_time = time(NULL);  // ��¼ȡ��ʱ��
                strcpy(current->pickup_name,
                       current->username);  // ��¼ȡ��������
                printf("����ȡ���ɹ�\n");

                // ȡ�����Զ����浽�ļ�
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("�����������ʧ��\n");
                }

                return 1;
            } else if (current->status == STRANDED) {
                int stranded_days = current->stranded_time;
                float stranded_fee = stranded_days * 1.5f;
                printf("\n����������%d��\n", stranded_days);
                printf("----------------------------------------\n");
                printf("����������ϸ:\n");
                printf("������������: %.2fԪ/��\n", 1.5f);
                printf("��������: %d��\n", stranded_days);
                printf("����������: %.2fԪ\n", stranded_fee);
                printf("----------------------------------------\n");
                printf("�Ƿ�֧���������ú����ȡ����(y/n): ");
                char confirm = getValidatedCharInput("YyNn");
                if (confirm == 'y' || confirm == 'Y') {
                    current->status = PICKED_UP;
                    current->pickup_time = time(NULL);  // ��¼ȡ��ʱ��
                    strcpy(current->pickup_name,
                           current->username);  // ��¼ȡ��������
                    printf("��������ȡ���ɹ�\n");
                    // ȡ�����Զ����浽�ļ�
                    if (!savePackagesToFile(system, PACKAGE_FILE)) {
                        printf("�����������ʧ��\n");
                    }
                    return 1;
                } else {
                    printf("��������ȡ��ȡ��\n");
                    return 0;
                }
            } else {
                printf("�˰����޷�ȡ��\n");
                return 0;
            }
        }
        current = current->next;
    }
    printf("δ�ҵ�����\n");
    return 0;
}

// ������а�������
void clearAllPackages(PackageSystem* system) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    PackageNode* current = system->head;
    PackageNode* next;

    // �ͷ����нڵ���ڴ�
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // ����ϵͳ״̬
    system->head = NULL;
    system->package_count = 0;

    // ��������ļ�
    FILE* file = fopen(PACKAGE_FILE, "wt");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("������������ļ�ʧ��\n");
    }
}

char* packageSatatusToString(PackageStatus status) {
    switch (status) {
        case PICKED_UP:
            return (char*)"��ȡ��";
            break;
        case PENDING_DELIVERY:
            return (char*)"���ĳ�";
            break;
        case DELIVERED:
            return (char*)"�Ѽĳ�";
            break;
        case PENDING_PICKUP:
            return (char*)"��ȡ��";
            break;
        case ABNORMAL:
            return (char*)"�쳣";
            break;
        case REJECTED:
            return (char*)"����";
            break;
        case STRANDED:
            return (char*)"����";
        case PICKED_BY_OTHER:
            return (char*)"����ȡ";
            break;
        case CLEANED:
            return (char*)"������";
            break;
        default:
            return (char*)"δ֪";
            break;
    }
}

void handleStrandedPackages(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // ��ʾ������������
    printf("\n----------------------------------------");
    printf("\n���������б�:\n");
    printf("����ID\t�����û�\t\t����ʱ��(��)\n");
    printf("----------------------------------------\n");
    time_t current_time = time(NULL);
    int found = 0;
    PackageNode* package = system->head;
    while (package != NULL) {
        if (package->status == STRANDED) {
            printf("%d\t%s\t\t\t%d\n", package->package_id, package->username,
                   package->stranded_time);
            found = 1;
        }
        package = package->next;
    }

    if (!found) {
        printf("��ǰû����������\n");
        return;
    }
    printf("\n----------------------------------------");
    printf("\n����ѡ��:\n");
    printf("1. ��ʱ�䴦����������\n");
    printf("2. ������ID����\n");
    printf("0. �������˵�\n");
    printf("----------------------------------------\n");
    int choice = getValidatedIntegerInput(0, 2, 1);

    switch (choice) {
        case 1: {
            printf("������Ҫ�������������(3-30): ");
            int days = getValidatedIntegerInput(3, 30, 1);
            processStrandedPackages(system, days, 0);
            break;
        }
        case 2: {
            printf("������Ҫ����İ���ID: ");
            int package_id = getValidatedIntegerInput(1000, 9999, 1);
            processStrandedPackages(system, 0, package_id);
            break;
        }
    }
}

void processStrandedPackages(PackageSystem* system, int days, int package_id) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    int processed_count = 0;
    PackageNode* package = system->head;

    while (package != NULL) {
        if (package->status == STRANDED) {
            // ���������������
            if ((days > 0 && package->stranded_time >= days) ||
                (package_id > 0 && package->package_id == package_id)) {
                package->status = CLEANED;
                processed_count++;
            }
        }
        package = package->next;
    }

    if (processed_count > 0) {
        if (savePackagesToFile(system, PACKAGE_FILE)) {
            printf("�ѳɹ����� %d ����������\n", processed_count);
        } else {
            printf("�����������ʧ��\n");
        }
    } else {
        printf("δ�ҵ�������������������\n");
    }
}
int generatePackageId(PackageSystem* system) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return 0;
    }

    // ʹ�õ�ǰʱ����Ϊ���������
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand((unsigned int)time(NULL));
        seed_initialized = 1;
    }

    int new_id;
    int id_found = 0;
    PackageNode* current;

    // �������ID��ȷ�����ظ�
    while (!id_found) {
        // ����1000-9999��Χ�ڵ������
        new_id = 1000 + (rand() % 9000);
        int id_exists = 0;
        current = system->head;

        // ���ID�Ƿ��Ѵ���
        while (current != NULL) {
            if (current->package_id == new_id) {
                id_exists = 1;
                break;
            }
            current = current->next;
        }

        if (!id_exists) {
            id_found = 1;
        }
    }

    return new_id;
}
void handleAddPackage(PackageSystem* system, UserSystem* user_system) {
    int size_choice, urgency_choice, type_choice, storage_choice;
    float weight_kg;
    char description[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];

    UserNode* current = user_system->head;
    UserType user_type;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            user_type = current->type;
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                // Administrator and courier users can select user
                printf("\n��ѡ�û��б�\n");
                UserNode* user = user_system->head;
                while (user != NULL) {
                    printf("%s\n", user->username);
                    user = user->next;
                }
                printf("��������������ߵ��û���: ");
                getValidatedStringInput(username, MAX_USERNAME_LENGTH);

                // ��֤�û����Ƿ����
                int user_found = 0;
                user = user_system->head;
                while (user != NULL) {
                    if (strcmp(user->username, username) == 0) {
                        user_found = 1;
                        break;
                    }
                    user = user->next;
                }
                if (!user_found) {
                    printf("�û���������\n");
                    return;
                }
            } else {
                // Non-admin users can only add to their own account
                strncpy(username, current->username, MAX_USERNAME_LENGTH);
            }
            break;
        }
        current = current->next;
    }

    printf("�������������: ");
    getValidatedStringInput(description, MAX_NAME_LENGTH);

    printf("��ѡ�������С (0:С��, 1:�м�, 2:���): ");
    size_choice = getValidatedIntegerInput(0, 2, 1);

    printf("�������������(kg): ");
    weight_kg = getValidatedFloatInput(0.1f, 100.0f);
    // Ϊ��������Ҳ����������100
    printf("��ѡ������̶� (0:��ͨ, 1:�Ӽ�): ");
    urgency_choice = getValidatedIntegerInput(0, 1, 1);

    printf("��ѡ��������� (0:��ͨ����, 1:������Ʒ, 2:������Ʒ): ");
    type_choice = getValidatedIntegerInput(0, 2, 1);

    printf("��ѡ��洢Ҫ�� (0:����, 1:���): ");
    storage_choice = getValidatedIntegerInput(0, 1, 1);

    char recipient[MAX_USERNAME_LENGTH];
    float delivery_fee = 0.0f;
    char adress[MAX_ADDRESS_LENGTH];
    if ((user_type != USER_ADMIN) && (user_type != USER_COURIER)) {
        printf("�������ռ�������: ");
        getValidatedStringInput(recipient, MAX_USERNAME_LENGTH);
        printf("�������ռ��˵�ַ: ");
        getValidatedStringInput(adress, MAX_ADDRESS_LENGTH);

        delivery_fee =
            calculateDeliveryFee(weight_kg, size_choice, urgency_choice,
                                 type_choice, storage_choice, user_type);
        pauseAndClearConsole(0);
        printf("----------------------------------------\n");
        printf("���ͷ�����ϸ:\n");
        printf("----------------------------------------\n");
        printf("�����˷� (%.2f Ԫ/kg):\t%.2f Ԫ\n", 0.8f, weight_kg * 0.8f);

        float base_fee = weight_kg * 0.8f;
        float size_fee = 0.0f, urgency_fee = 0.0f, type_fee = 0.0f,
              storage_fee = 0.0f;

        // Size-based fee
        switch (size_choice) {
            case 1:
                size_fee = base_fee * 0.5f;
                printf("�м��������ӷ� (50%%):\t\t%.2f Ԫ\n", size_fee);
                break;
            case 2:
                size_fee = base_fee * 1.0f;
                printf("����������ӷ� (100%%):\t\t%.2f Ԫ\n", size_fee);
                break;
        }

        // Urgency fee
        if (urgency_choice == 1) {
            urgency_fee = (base_fee + size_fee) * 0.5f;
            printf("�Ӽ������ (50%%):\t%.2f Ԫ\n", urgency_fee);
        }

        // Package type fee
        if (type_choice == 1 || type_choice == 2) {
            type_fee = (base_fee + size_fee + urgency_fee) * 0.5f;
            printf("%s���ӷ� (50%%):\t%.2f Ԫ\n",
                   type_choice == 1 ? "������Ʒ" : "������Ʒ", type_fee);
        }

        // Storage requirement fee
        if (storage_choice == 1) {
            storage_fee = (base_fee + size_fee + urgency_fee + type_fee) * 0.5f;
            printf("��ط���� (50%%):\t%.2f Ԫ\n", storage_fee);
        }

        float subtotal =
            base_fee + size_fee + urgency_fee + type_fee + storage_fee;
        printf("----------------------------------------\n");
        printf("С��:\t\t\t%.2f Ԫ\n", subtotal);

        // User discount
        float discount = 0.0f;
        if (user_type != USER_NORMAL && user_type != USER_ADMIN) {
            discount = subtotal * (1.0f - (user_type == USER_STUDENT   ? 0.8f
                                           : user_type == USER_TEACHER ? 0.7f
                                           : user_type == USER_VIP     ? 0.6f
                                                                       : 1.0f));
            printf("�û��ۿ� (%s):\t-%.2f Ԫ\n",
                   user_type == USER_STUDENT   ? "����"
                   : user_type == USER_TEACHER ? "����"
                   : user_type == USER_VIP     ? "����"
                                               : "���ۿ�",
                   discount);
        }

        printf("----------------------------------------\n");
        printf("�ܽ��:\t\t%.2f Ԫ\n", delivery_fee);
        printf("----------------------------------------\n");

        char confirm;
        printf("----------------------------------------\n");
        printf("ȷ��֧�����洢������(y/n): ");
        confirm = getValidatedCharInput("YNyn");
        if (confirm != 'y' && confirm != 'Y') {
            printf("�����洢��ȡ��\n");
            return;
        }
    }

    int result = addPackage(
        system, username, description, (PackageSize)size_choice, weight_kg,
        (PackageUrgency)urgency_choice, (PackageType)type_choice,
        (PackageStorage)storage_choice, ABNORMAL);

    if (result > 0) {
        PackageNode* package = system->head;
        while (package != NULL) {
            if (package->package_id == result) {
                if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                    package->status = PENDING_PICKUP;
                } else {
                    package->status = PENDING_DELIVERY;
                }

                if ((user_type != USER_ADMIN) && (user_type != USER_COURIER)) {
                    strncpy(package->recipient, recipient,
                            MAX_USERNAME_LENGTH - 1);
                    package->recipient[MAX_USERNAME_LENGTH - 1] = '\0';
                    strncpy(package->recipientAdress, adress,
                            MAX_ADDRESS_LENGTH - 1);
                    package->recipientAdress[MAX_ADDRESS_LENGTH - 1] = '\0';
                    package->delivery_fee = delivery_fee;
                }
                break;
            }
            package = package->next;
        }
        savePackagesToFile(system, PACKAGE_FILE);
        printf("----------------------------------------\n");
        printf("������ӳɹ���\n");
        printf("����ID: %d\n", result);
        if (user_type == USER_ADMIN || user_type == USER_COURIER) {
            printf("״̬: ��ȡ��\n");
        } else {
            printf("״̬: ���ĳ�\n");
        }
        // Ĭ�Ͽ�ݺ͹���Ա����Ķ��Ǵ�ȡ���������û��Ǵ��ĳ�
        printf("----------------------------------------\n");
        if (user_type != USER_ADMIN && user_type != USER_COURIER) {
            printf("������: %s\n", username);
            printf("�ռ���: %s\n", recipient);
            printf("��ַ: %s\n", adress);
            printf("��ݷ���: %.2f Ԫ\n", delivery_fee);
            printf("----------------------------------------\n");
        }
    } else {
        printf("��Ӱ���ʧ��\n");
    }
}
void handleQueryPackages(PackageSystem* system, UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];

    // ��ȡ��ǰ��¼�û����û���������

    UserNode* current = user_system->head;
    UserType user_type;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            user_type = current->type;
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                getSpecificUser(user_system, username);
            } else {
                // ��ͨ�û�ֻ�ܲ鿴�Լ��İ���
                strncpy(username, current->username, MAX_USERNAME_LENGTH);
            }

            break;
        }
        current = current->next;
    }
    queryPackagesByUsername(system, username);
}
void handlePickupPackage(PackageSystem* system, UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];

    UserNode* current = user_system->head;
    UserType user_type;

    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            user_type = current->type;
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                getSpecificUser(user_system, username);
            } else {
                // ��ͨ�û�ֻ�ܲ鿴�Լ��İ���
                strncpy(username, current->username, MAX_USERNAME_LENGTH);
            }
            break;
        }
        current = current->next;
    }

    queryPackagesByUsername(system, username);

    int package_id;
    printf("\n������Ҫȡ���İ���ID������0���أ���");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id != 0) {
        pickupPackage(system, package_id);
    }
}

void handleDeliverPackage(PackageSystem* system) {
    int package_id;
    int found_pending = 0;

    printf("\n���ĳ�/���ܾ��İ����б�:\n");
    printf("����ID\t����\t\t�ռ���\t\t���ͷ���\n");

    PackageNode* current = system->head;
    while (current != NULL) {
        // ����Ƿ��ڴ�����״̬
        if (current->status == PENDING_DELIVERY ||
            current->status == REJECTED) {  // ������״̬
            printf("%d\t%s\t\t%s\t\t%.2f\n", current->package_id,
                   current->description, current->recipient,
                   current->delivery_fee);
            found_pending = 1;
        }
        current = current->next;
    }

    if (!found_pending) {
        printf("û�д��ĳ��İ���\n");
        return;
    }

    printf("\n������Ҫ�ĳ��İ���ID: ");
    package_id = getValidatedIntegerInput(1000, 9999, 0);

    current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            // �������Ƿ��ڴ�����״̬������Ϊ������
            if (current->status == PENDING_DELIVERY) {
                current->status = DELIVERED;      // ����Ϊ������״̬
                current->sent_time = time(NULL);  // ��¼����ʱ��
                printf("�����ѳɹ��ĳ�\n");

                // ���ͺ��Զ����浽�ļ�
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("�����������ʧ��\n");
                }
                return;
            } else {
                printf("�˰��������ڴ��ĳ�״̬\n");
                return;
            }
        }
        current = current->next;
    }
    printf("δ�ҵ�����\n");
}
int handleClearSystemData(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return 0;
    }

    char confirm;
    printf("����: �˲�������������û��Ͱ����Լ��������ݡ�������(y/n): ");
    confirm = getValidatedCharInput("YNyn");

    if (confirm == 'y' || confirm == 'Y') {
        clearAllPackages(system);
        clearAllUsers(user_system);
        FeedbackSystem* feedback_system = loadFeedbackFromFile();
        clearAllFeedback(feedback_system);
        printf("ϵͳ���������\n");
        return 1;
    } else {
        printf("������ȡ��\n");
        return 0;
    }
}

/**
 * �������쳣��������
 * @param system ��������ϵͳָ��
 * @param user_system �û�ϵͳָ��
 */
void handleMarkAbnormalPackage(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // ��ʾ�û��б�
    printf("\n��ѡ�û��б�\n");
    UserNode* user = user_system->head;
    while (user != NULL) {
        printf("%s\n", user->username);
        user = user->next;
    }

    // ѡ���û�
    char username[MAX_USERNAME_LENGTH];
    printf("������Ҫ���������û�����");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);

    // ��֤�û��Ƿ����
    int user_found = 0;
    user = user_system->head;
    while (user != NULL) {
        if (strcmp(user->username, username) == 0) {
            user_found = 1;
            break;
        }
        user = user->next;
    }
    if (!user_found) {
        printf("�û���������\n");
        return;
    }

    // ��ʾ���û��İ�����Ϣ
    printf("\n�û� %s �İ�����\n", username);
    printf("����ID\t����\t\t״̬\n");
    int found = 0;
    PackageNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("%d\t%-16s\t%s\n", current->package_id, current->description,
                   packageSatatusToString(current->status));
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("δ�ҵ����û��İ���\n");
        return;
    }

    // ѡ��Ҫ��ǵİ���
    int package_id;
    printf("\n������Ҫ���Ϊ�쳣�İ���ID������0ȡ������");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id == 0) {
        printf("������ȡ��\n");
        return;
    }

    // ���°���״̬
    current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            if (strcmp(current->username, username) == 0) {
                current->status = ABNORMAL;
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("�����������ʧ��\n");
                } else {
                    printf("�����ѳɹ����Ϊ�쳣״̬\n");
                }
                return;
            } else {
                printf("�ð���������ѡ���û�\n");
                return;
            }
        }
        current = current->next;
    }
    printf("δ�ҵ�����\n");
}
// �����������
// �����û����հ���������Ա/���Ա����ͨ�û��в�ͬ����ʾ�߼�
// ������system - ��������ϵͳָ�룬user_system - �û�����ϵͳָ��
void handleRejectPackage(PackageSystem* system, UserSystem* user_system) {
    // ���ϵͳ��ʼ��״̬
    if (system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // ��ȡ��ǰ�û���Ϣ

    UserType current_user_type = USER_NORMAL;
    char current_username[MAX_USERNAME_LENGTH];

    UserNode* current = user_system->head;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            current_user_type = current->type;
            strncpy(current_username, current->username, MAX_USERNAME_LENGTH);
            break;
        }
        current = current->next;
    }

    char target_username[MAX_USERNAME_LENGTH];
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        getSpecificUser(user_system, target_username);
    } else {
        // ��ͨ�û�ֻ�ܲ鿴�Լ��İ���
        strncpy(target_username, current->username, MAX_USERNAME_LENGTH);
    }
    // ��ʾѡ���û��İ�����Ϣ
    printf("\n----------------------------------------\n");
    printf("�û� %s �İ����б�:\n", target_username);
    printf("----------------------------------------\n");
    int found = 0;
    PackageNode* package = system->head;
    while (package != NULL) {
        if (strcmp(package->username, target_username) == 0) {
            printf("�������: %d\n", package->package_id);
            printf("��������: %s\n", package->description);
            printf("����״̬: %s\n", packageSatatusToString(package->status));
            printf("----------------------------------------\n");
            found = 1;
        }
        package = package->next;
    }

    if (!found) {
        printf("----------------------------------------\n");
        printf("���û�û�а���\n");
        printf("----------------------------------------\n");
        return;
    }

    // ����������ղ���
    int package_id;
    printf("������Ҫ���յİ������ (����0ȡ��): ");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id == 0) {
        printf("----------------------------------------\n");
        printf("������ȡ��\n");
        printf("----------------------------------------\n");
        return;
    }

    // ���Ҳ����°���״̬
    package = system->head;
    while (package != NULL) {
        if (package->package_id == package_id) {
            // ��֤��������Ȩ
            if (strcmp(package->username, target_username) != 0) {
                printf("�ð���������ѡ���û�\n");
                return;
            }

            // ������״̬�Ƿ���Ծ���
            if (package->status == PENDING_PICKUP ||
                package->status == ABNORMAL) {
                package->status = REJECTED;
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("�����������ʧ��\n");
                } else {
                    printf("----------------------------------------\n");
                    printf("�����ѳɹ�����\n");
                    printf("����״̬: %s\n",
                           packageSatatusToString(package->status));
                    printf("���ɿ��Ա�Ļ�ԭ��ַ\n");
                    printf("----------------------------------------\n");
                }
                return;
            } else {
                printf("----------------------------------------\n");
                printf("��ǰ״̬�µİ����޷�����\n");
                printf("----------------------------------------\n");
                return;
            }
        }
        package = package->next;
    }
    printf("----------------------------------------\n");
    printf("δ�ҵ��ð���\n");
    printf("----------------------------------------\n");
}

int generatePackageLocation(PackageSystem* system,
                            int* shelf_number,
                            int* layer_number) {
    // ������Ч�Լ��
    if (system == NULL || shelf_number == NULL || layer_number == NULL) {
        return 0;
    }

    // ʹ�õ�ǰʱ����Ϊ���������
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand((unsigned int)time(NULL));
        seed_initialized = 1;
    }

    int attempts = 0;
    const int MAX_ATTEMPTS = 50;  // ����λ����Ͻ��٣����ͳ��Դ���

    do {
        // ������ɻ��ܺ�(1-5)�Ͳ���(0-1)
        *shelf_number = (rand() % SHELF_COUNT) + 1;
        *layer_number = rand() % LAYER_COUNT;

        // ���λ���Ƿ��ѱ�ռ�û�����
        int location_occupied = 0;
        int packages_in_location = 0;
        PackageNode* current = system->head;

        while (current != NULL) {
            // ֻ���δȡ�ߺ�δ�ʹ�İ���
            if (current->status != PICKED_UP && current->status != DELIVERED &&
                current->shelf_number == *shelf_number &&
                current->layer_number == *layer_number) {
                packages_in_location++;
                if (packages_in_location >= LAYER_CAPACITY) {
                    location_occupied = 1;
                    break;
                }
            }
            current = current->next;
        }

        // ���λ��δ��ռ�������سɹ�
        if (!location_occupied) {
            printf(
                "��Ϊ��������λ�ã����� %d �� %d �㣨��ǰ��λ������ %d "
                "��������\n",
                *shelf_number, *layer_number + 1, packages_in_location);
            return 1;
        }

        attempts++;
    } while (attempts < MAX_ATTEMPTS);

    // ������Դ����ﵽ������δ�ҵ�����λ�ã�����ʧ��
    printf("���棺�޷��ҵ����õĴ洢λ�ã�����λ�ÿ�������\n");
    return 0;
}
void handlePickupPackageByOther(PackageSystem* system,
                                UserSystem* user_system) {
    if (system == NULL || user_system == NULL ||
        user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    // ���ҵ�ǰ�û��ڵ�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            break;
        }
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        printf("��ȡ�û���Ϣʧ��\n");
        return;
    }

    // ��ʾ�����б�
    printf("\n���ĺ����б�\n");
    printf("----------------------------------------\n");
    int found = 0;
    for (int i = 0; i < current_user->friend_count; i++) {
        printf("%s\n", current_user->friends[i]);
        found = 1;
    }

    if (!found) {
        printf("����û������κκ���\n");
        printf("----------------------------------------\n");
        return;
    }
    printf("----------------------------------------\n");

    // ��������û���
    char friend_username[MAX_USERNAME_LENGTH];
    printf("������Ҫ��ȡ��ݵĺ����û���������0���أ���");
    getValidatedStringInput(friend_username, MAX_USERNAME_LENGTH);

    if (strcmp(friend_username, "0") == 0) {
        return;
    }

    // ��֤������û����Ƿ��ں����б���
    int is_friend = 0;
    for (int i = 0; i < current_user->friend_count; i++) {
        if (strcmp(current_user->friends[i], friend_username) == 0) {
            is_friend = 1;
            break;
        }
    }

    if (!is_friend) {
        printf("\n���󣺸��û��������ĺ����б���\n");
        return;
    }

    // ��ʾ���ѵİ���
    printf("\n%s �İ����б�\n", friend_username);
    printf("----------------------------------------\n");
    queryPackagesByUsername(system, friend_username);

    // ����Ҫ��ȡ�İ���ID
    int package_id;
    printf("\n������Ҫ��ȡ�İ���ID������0���أ���");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id != 0) {
        pickupPackageByOther(system, package_id, current_user);
    }
}

void pickupPackageByOther(PackageSystem* system,
                          int package_id,
                          UserNode* current) {
    if (system == NULL) {
        printf("ϵͳ����\n");
        return;
    }

    PackageNode* package = system->head;
    while (package != NULL) {
        if (package->package_id == package_id) {
            // ������״̬
            if (package->status != PENDING_PICKUP &&
                package->status != ABNORMAL) {
                printf("----------------------------------------\n");
                printf("�ð�����ǰ״̬�޷���ȡ\n");
                printf("��ǰ״̬��%s\n",
                       packageSatatusToString(package->status));
                printf("----------------------------------------\n");
                return;
            }

            // ���°���״̬Ϊ�Ѵ�ȡ
            package->status = PICKED_BY_OTHER;
            package->pickup_time = time(NULL);
            strcpy(package->pickup_name, current->username);
            if (!savePackagesToFile(system, PACKAGE_FILE)) {
                printf("���������Ϣʧ��\n");
            } else {
                printf("----------------------------------------\n");
                printf("�����ѳɹ���ȡ\n");
                printf("����״̬��%s\n",
                       packageSatatusToString(package->status));
                printf("----------------------------------------\n");
            }
            return;
        }
        package = package->next;
    }

    printf("----------------------------------------\n");
    printf("δ�ҵ��ð���\n");
    printf("----------------------------------------\n");
}
void displayPackageDetails(PackageNode* current) {
    char store_time_str[30] = "";
    char pickup_time_str[30] = "";
    char sent_time_str[30] = "";
    struct tm* timeinfo;

    timeinfo = localtime(&current->store_time);
    strftime(store_time_str, sizeof(store_time_str), "%m-%d %H:%M:%S",
             timeinfo);

    if (current->pickup_time > 0) {
        timeinfo = localtime(&current->pickup_time);
        strftime(pickup_time_str, sizeof(pickup_time_str), "%m-%d %H:%M:%S",
                 timeinfo);
    } else {
        strcpy(pickup_time_str, "-");
    }
    printf("\n������ϸ��Ϣ:\n");
    printf("����ID: %d\n", current->package_id);
    printf("������: %s\n", current->username);
    printf("״̬: %s\n", packageSatatusToString(current->status));
    printf("����: %s\n", current->description);

    printf("ȡ����: %s\n", current->pickup_code);
    printf("λ��: %d�Ż���, %d��\n", current->shelf_number,
           current->layer_number);
    if (current->status == PENDING_DELIVERY) {
        printf("�ռ���: %s\n", current->recipient);
        printf("�ռ��˵�ַ: %s\n", current->recipientAdress);
        printf("���ͷ���: %.2f\n", current->delivery_fee);
    }

    printf("�洢ʱ��: %s\n", store_time_str);
    if (current->status == PICKED_UP || current->status == PICKED_BY_OTHER) {
        printf("ȡ��ʱ��: %s\n", pickup_time_str);
        printf("ȡ����: %s\n", current->pickup_name);
    } else if (current->status == DELIVERED) {
        if (current->sent_time > 0) {
            timeinfo = localtime(&current->sent_time);
            strftime(sent_time_str, sizeof(sent_time_str), "%m-%d %H:%M:%S",
                     timeinfo);
            printf("�ĳ�ʱ��: %s\n", sent_time_str);
        }
    }
    printf("----------------------------------------\n");
}