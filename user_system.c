#include "all_h_files.h"
#define ADMIN_KEY "admin"
// ��ʼ���û�����ϵͳ
// �����ڴ沢��ʼ��ϵͳ�Ļ������ݽṹ
// ����ֵ���ɹ�����ϵͳָ�룬ʧ�ܷ���NULL
UserSystem* initUserSystem() {
    // Ϊϵͳ�ṹ������ڴ�
    UserSystem* system = (UserSystem*)malloc(sizeof(UserSystem));
    if (system == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return NULL;
    }
    // ��ʼ��ϵͳ״̬
    system->head = NULL;       // �û�����ͷָ���ʼ��Ϊ��
    system->user_count = 0;    // �û�������ʼ��Ϊ0
    system->is_login = false;  // ��ǰ��¼�û�is_login״̬��ʼ��Ϊfalse
    return system;
}

// ���ļ������û�����
// ��ȡ�������ļ��е��û���Ϣ���ؽ��û�����
// ������system - ϵͳָ�룬filename - �����ļ���
// ����ֵ���ɹ�����1��ʧ�ܷ���0
int loadUsersFromFile(UserSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }
    // ���ı�ģʽ���ļ�
    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return 0;
    }
    // ע���ļ���һ��λ�ô���û�����
    //  ��ȡ�û�����
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // ��ȡ�û����ݲ��ؽ�����
    UserNode temp;
    system->head = NULL;
    system->user_count = 0;

    // ѭ����ȡÿ���û�������
    for (int i = 0; i < count; i++) {
        // ��ȡһ���û��ڵ������
        if (fread(&temp, sizeof(UserNode), 1, file) != 1) {
            // ��ȡʧ��
            fclose(file);
            return 0;
        }

        // Ϊ�½ڵ�����ڴ�
        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        if (new_node == NULL) {
            fclose(file);
            return 0;
        }
        // ע������temp��һ��ʵʵ���ڵĽڵ㣬����ָ��
        //  �������ݵ��½ڵ�
        *new_node = temp;
        new_node->next = NULL;

        // ���½ڵ���ӵ�����ĩβ
        if (system->head == NULL) {
            system->head = new_node;
        } else {
            UserNode* current = system->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = new_node;
        }
        system->user_count++;
    }

    fclose(file);
    return 1;
}

// �����û����ݵ��ļ�
// ����ǰ�û������е������û���Ϣд��������ļ�
// ������system - ϵͳָ�룬filename - �����ļ���
// ����ֵ���ɹ�����1��ʧ�ܷ���0
int saveUsersToFile(UserSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    // ���ı�дģʽ���ļ�
    FILE* file = fopen(filename, "wt");
    if (file == NULL) {
        return 0;
    }

    // д���û�����
    if (fwrite(&system->user_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // ���д���û�����
    UserNode* current = system->head;
    while (current != NULL) {
        if (fwrite(current, sizeof(UserNode), 1, file) != 1) {
            fclose(file);
            return 0;
        }
        current = current->next;
    }
    fclose(file);
    return 1;
}

// ע�����û�
// �������û�����ӵ�ϵͳ�У�ͬʱ���浽�ļ�
// ������system - ϵͳָ�룬username - �û�����password - ���룬type - �û�����
// ����ֵ���ɹ������û�����ʧ�ܷ���NULL
char* registerUser(UserSystem* system,
                   const char* username,
                   const char* password,
                   UserType type) {
    if (system == NULL || username == NULL || password == NULL) {
        return NULL;
    }
    // ����û����Ƿ��Ѵ��ڣ�����ָ��ָ������ĩβ
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("�û����Ѵ��ڣ�\n");
            return NULL;
        }
        current = current->next;
    }
    // �������û��ڵ�
    UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
    if (new_node == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return NULL;
    }
    strncpy(new_node->username, username, MAX_USERNAME_LENGTH - 1);
    new_node->username[MAX_USERNAME_LENGTH - 1] = '\0';
    strncpy(new_node->password, password, MAX_PASSWORD_LENGTH - 1);
    new_node->password[MAX_PASSWORD_LENGTH - 1] = '\0';
    new_node->type = type;
    new_node->friend_count = 0;
    new_node->friends[0][0] = '\0';  // ��ʼ�� friends[0]
    // Ϊע����û����������������
    strcpy(new_node->invateCode, generateInviteCode());
    new_node->next = NULL;
    // ���½ڵ���ӵ�����ĩβ
    if (system->head == NULL) {
        system->head = new_node;
    } else {
        current = system->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    system->user_count++;

    // ע��ɹ��������������ݱ�֤���ݲ���ʧ
    if (!saveUsersToFile(system, USER_FILE)) {
        printf("�����û�����ʧ�ܣ�\n");
    }
    return new_node->username;
}

// �û���¼
// ��֤�û��������룬���õ�ǰ��¼�û�
// ������system - ϵͳָ�룬username - �û�����password - ����
// ����ֵ���ɹ�����1��ʧ�ܷ���0
int loginUser(UserSystem* system, const char* username, const char* password) {
    if (system == NULL || username == NULL || password == NULL) {
        return 0;
    }

    // �����û��������ƥ����û���������
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 &&
            strcmp(current->password, password) == 0) {
            strcpy(system->current_username, username);
            system->is_login = true;
            pauseAndClearConsole(0);
            printf("----------------------------------------\n");
            printf("��¼�ɹ����û����ͣ�%s\n", USER_TYPE_NAMES[current->type]);
            return 1;
        }
        current = current->next;
    }
    printf("�û�����������Ч\n");
    return 0;
}
// �û��ǳ�
// �����ǰ��¼�û���״̬
void logoutUser(UserSystem* system) {
    if (system != NULL) {
        system->is_login = false;
    }
    if (system->is_login == false) {
        printf("���˳���¼\n");
        printf("----------------------------------------\n");
    }
}

// ��������û�����
// �ͷ������ڴ沢����ϵͳ״̬
void clearAllUsers(UserSystem* system) {
    if (system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // �ͷ����нڵ���ڴ�
    UserNode* current = system->head;
    UserNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // ����ϵͳ״̬
    system->head = NULL;
    system->user_count = 0;
    system->is_login = false;

    // ��������ļ�
    FILE* file = fopen(USER_FILE, "wt");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("����û������ļ�ʧ��\n");
    }
}

// �޸��û�����
// ��֤ԭ���벢����Ϊ������
// ������system - ϵͳָ�룬username - �û���
// ����ֵ���ɹ�����1��ʧ�ܷ���0
int changeUserPassword(UserSystem* system, const char* username) {
    if (system == NULL || username == NULL) {
        return 0;
    }

    // �����û�
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            char old_password[MAX_PASSWORD_LENGTH];
            char new_password[MAX_PASSWORD_LENGTH];
            char confirm_password[MAX_PASSWORD_LENGTH];
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;  // ����Դ���

            // ��֤ԭ���룬�������3�γ���
            while (attempts < MAX_ATTEMPTS) {
                printf("\n������ԭ���� (��ʣ %d �γ��Ի���): ",
                       MAX_ATTEMPTS - attempts);
                getValidatedStringInput(old_password, MAX_PASSWORD_LENGTH);

                if (strcmp(current->password, old_password) == 0) {
                    // ԭ������֤�ɹ��������޸���������
                    printf("\nԭ������֤�ɹ���\n");

                    // ��ȡ������
                    printf("\n������������: ");
                    getValidatedStringInput(new_password, MAX_PASSWORD_LENGTH);

                    printf("��ȷ��������: ");
                    getValidatedStringInput(confirm_password,
                                            MAX_PASSWORD_LENGTH);

                    // ��֤����������������Ƿ�һ��
                    if (strcmp(new_password, confirm_password) != 0) {
                        printf("\n��������������벻һ�£������޸�ʧ��\n");
                        return 0;
                    }

                    // ��������
                    strncpy(current->password, new_password,
                            MAX_PASSWORD_LENGTH - 1);
                    current->password[MAX_PASSWORD_LENGTH - 1] = '\0';

                    // ������ĵ��ļ�
                    if (!saveUsersToFile(system, USER_FILE)) {
                        printf("���������޸�ʧ��\n");
                        return 0;
                    }

                    return 1;
                } else {
                    attempts++;
                    if (attempts < MAX_ATTEMPTS) {
                        printf("\nԭ������󣡻�ʣ %d �γ��Ի���\n",
                               MAX_ATTEMPTS - attempts);
                    } else {
                        printf("\n���� %d ��������֤ʧ�ܣ��˻������ǳ�\n",
                               MAX_ATTEMPTS);
                        logoutUser(system);
                        return 0;
                    }
                }
            }
            return 0;
        }
        current = current->next;
    }

    printf("δ�ҵ��û�\n");
    return 0;
}

// �����޸�������û�����
// ʹ�ð�ȫ��������֤������ȡ�û�����
void handleChangePassword(UserSystem* system) {
    if (system == NULL || system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    strncpy(username, system->current_username, MAX_USERNAME_LENGTH);
    if (username == NULL) {
        printf("��ȡ�û���Ϣʧ��\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("�޸�����\n");
    printf("----------------------------------------\n");

    // �����޸����뺯��
    if (changeUserPassword(system, username)) {
        printf("----------------------------------------\n");
        printf("�����޸ĳɹ����´ε�¼��ʹ��������\n");
        printf("----------------------------------------\n");
    } else {
        if (system->is_login == false) {
            printf("----------------------------------------\n");
            printf("���ڶ��������֤ʧ�ܣ��˻��ѱ��ǳ�\n");
            printf("----------------------------------------\n");
        } else {
            printf("----------------------------------------\n");
            printf("�����޸�ʧ��\n");
            printf("----------------------------------------\n");
        }
    }
}

// �û�ע��
// ��ϵͳ��ɾ����ǰ��¼�û����˻�
// ������system - �û�����ϵͳָ��
// ����ֵ���ɹ�����1��ʧ�ܷ���0
int deleteUserAccount(UserSystem* system) {
    if (system == NULL || system->is_login == false) {
        printf("���ȵ�¼\n");
        return 0;
    }

    // ��ȡ��ǰ�û���Ϣ
    UserNode* current = system->head;
    UserNode* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->username, system->current_username) == 0) {
            // ��֤�û����
            char password[MAX_PASSWORD_LENGTH];
            printf("\n----------------------------------------\n");
            printf("�˻�ע��\n");
            printf("----------------------------------------\n");
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;  // ����Դ���
            char old_password[MAX_PASSWORD_LENGTH];
            // ��֤���룬�������3�γ���
            while (attempts < MAX_ATTEMPTS) {
                printf("\n���������� (��ʣ %d �γ��Ի���): ",
                       MAX_ATTEMPTS - attempts);
                getValidatedStringInput(old_password, MAX_PASSWORD_LENGTH);

                if (strcmp(current->password, old_password) == 0) {
                    // ԭ������֤�ɹ�������ע���˻�
                    printf("\n��ǰ�˻�������֤�ɹ���\n");
                    // �ٴ�ȷ��
                    char confirm;
                    printf("\n���棺�˲���������ɾ�������˻����Ҳ��ɻָ���\n");
                    printf("ȷ��ע���˻���(y/n): ");
                    confirm = getValidatedCharInput("YNyn");

                    if (confirm != 'y' && confirm != 'Y') {
                        printf("������ȡ��\n");
                        printf("----------------------------------------\n");
                        return 0;
                    }
                    // ��������ɾ���û��ڵ�
                    if (prev == NULL) {
                        system->head = current->next;
                    } else {
                        prev->next = current->next;
                    }

                    // ����ϵͳ״̬
                    system->user_count--;
                    system->is_login = false;

                    // �ͷ��ڴ�
                    free(current);

                    // ������ĵ��ļ�
                    if (!saveUsersToFile(system, USER_FILE)) {
                        printf("�����û�����ʧ��\n");
                        printf("----------------------------------------\n");
                        return 0;
                    }

                    printf("�˻��ѳɹ�ע��\n");
                    printf("----------------------------------------\n");
                    return 1;
                } else {
                    attempts++;
                    if (attempts < MAX_ATTEMPTS) {
                        printf("\n������󣡻�ʣ %d �γ��Ի���\n",
                               MAX_ATTEMPTS - attempts);
                    } else {
                        printf("\n���� %d ��������֤ʧ�ܣ��˻������ǳ�\n",
                               MAX_ATTEMPTS);
                        logoutUser(system);
                        return 0;
                    }
                }
            }
        }
        prev = current;
        current = current->next;
    }

    printf("δ�ҵ��û���Ϣ\n");
    printf("----------------------------------------\n");
    return 0;
}
// �����û�ע��
// �����û�����ע����Ϣ�������û�����������û�����
// �Բ�ͬ���͵��û�������Ӧ����֤
// ������user_system - �û�����ϵͳָ��
void handleUserRegister(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int type_choice;
    // ��ȡ����ע����Ϣ
    printf("�������û���: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);

    UserNode* current = user_system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("�û����Ѵ��ڣ�\n");
            printf("ע��ʧ�ܣ�������\n");
            return;
        }
        current = current->next;
    }

    printf("����������: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    // ��ʾ�û�����ѡ��
    printf("----------------------------------------\n");
    printf("��ѡ���û�����:\n");
    printf("0. ѧ���û�\n");
    printf("1. ��ʦ�û�\n");
    printf("2. VIP�û�\n");
    printf("3. ��ͨ�û�\n");
    printf("4. ����Ա�û�\n");
    printf("----------------------------------------\n");
    printf("��ѡ�� (0-4): ");
    type_choice = getValidatedIntegerInput(0, 4, 1);

    // ���ݲ�ͬ�û����ͽ���������֤
    if (type_choice == USER_STUDENT || type_choice == USER_TEACHER) {
        // ѧ���ͽ�ʦ��Ҫ��֤��������
        char email[100];
        printf("�����뼪������ (��ʽ: xxx@jlu.edu.cn): ");
        getValidatedStringInput(email, 100);

        char* at_pos = strstr(email, "@jlu.edu.cn");
        if (at_pos == NULL || strcmp(at_pos, "@jlu.edu.cn") != 0) {
            printf("�����ʽ��Ч��ע��ʧ��\n");
            return;
        }
        printf("������֤�ɹ���\n");
    } else if (type_choice == USER_VIP) {
        // VIP�û���Ҫȷ�����֧��
        char pay_confirm;
        printf("VIP�û���Ҫ֧�����200Ԫ��ȷ��֧����(y/n): ");
        pay_confirm = getValidatedCharInput("YNyn");

        if (pay_confirm != 'y' && pay_confirm != 'Y') {
            printf("֧��δ��ɣ�ע��ʧ��\n");
            return;
        }
        printf("֧���ɹ���\n");
    } else if (type_choice == USER_ADMIN) {
        // ����Ա��Ҫ��֤����Ա��Կ
        char admin_key[50];
        printf("���������Ա��Կ: ");
        getValidatedStringInput(admin_key, 50);
        if (strcmp(admin_key, ADMIN_KEY) != 0) {
            printf("����Ա��Կ��Ч��ע��ʧ��\n");
            return;
        }
    }

    // ִ���û�ע��
    if (registerUser(user_system, username, password, (UserType)type_choice)) {
        printf("----------------------------------------\n");
        printf("ע��ɹ����û�����: %s\n", USER_TYPE_NAMES[type_choice]);
    } else {
        printf("ע��ʧ�ܣ�������\n");
    }
}

// �����û���¼
// ��֤�û���¼��Ϣ�������û���������
// ������user_system - �û�����ϵͳָ��
void handleUserLogin(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("�������û���: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);
    printf("����������: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    if (loginUser(user_system, username, password)) {
    } else {
        printf("��¼ʧ�ܣ������û���������\n");
        pauseAndClearConsole(1);
    }
}

// ������Աע��
// ��������Աʹ�ã����ڴ����µĿ��Ա�˻�
// ������user_system - �û�����ϵͳָ��
void handleCourierRegister(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("��������Ա�û���: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);
    printf("��������Ա����: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    if (registerUser(user_system, username, password, USER_COURIER)) {
        printf("���Ա�˻�ע��ɹ���\n");
    } else {
        printf("���Ա�˻�ע��ʧ�ܣ�������\n");
    }
}
