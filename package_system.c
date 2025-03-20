#include "all_h_files.h"
// 从文件加载包裹数据
int loadPackagesFromFile(PackageSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return 0;
    }

    // 读取包裹数量
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // 逐个读取包裹数据并构建链表
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

        // 将新节点添加到链表末尾
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

// 保存包裹数据到文件
int savePackagesToFile(PackageSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    FILE* file = fopen(filename, "wt");
    if (file == NULL) {
        return 0;
    }

    // 写入包裹数量
    if (fwrite(&system->package_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // 逐个写入包裹数据
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

// 初始化包裹管理系统
PackageSystem* initPackageSystem() {
    PackageSystem* system = (PackageSystem*)malloc(sizeof(PackageSystem));
    if (system == NULL) {
        printf("内存分配失败\n");
        return NULL;
    }
    system->head = NULL;
    system->package_count = 0;
    return system;
}

// 添加新包裹
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
        printf("系统未初始化\n");
        return 0;
    }

    PackageNode* new_node = (PackageNode*)malloc(sizeof(PackageNode));
    if (new_node == NULL) {
        printf("内存分配失败\n");
        return 0;
    }

    // 使用专用生成器生成包裹ID
    int new_id = generatePackageId(system);
    if (new_id == 0) {
        free(new_node);
        return 0;
    }

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

    // 使用包裹位置生成器分配货架号和层号
    int shelf_number, layer_number;
    if (!generatePackageLocation(system, &shelf_number, &layer_number)) {
        free(new_node);
        return 0;
    }
    new_node->shelf_number = shelf_number;
    new_node->layer_number = layer_number;

    // 生成取件码
    snprintf(new_node->pickup_code, MAX_PICKUP_CODE_LENGTH, "%d-%d-%d",
             new_node->shelf_number, new_node->layer_number,
             new_node->package_id);

    // 记录存储时间
    new_node->store_time = time(NULL);
    new_node->pickup_time = 0;  // 初始化取件时间为0

    new_node->next = NULL;

    // 将新节点添加到链表末尾
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

    // 添加包裹后自动保存到文件
    if (!savePackagesToFile(system, PACKAGE_FILE)) {
        printf("保存包裹数据失败\n");
    }

    return new_node->package_id;
}

// 根据用户名查询包裹
void queryPackagesByUsername(PackageSystem* system, const char* username) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    int found = 0;
    printf("----------------------------------------\n");
    printf("用户 %s 的包裹信息:\n", username);
    PackageNode* current = system->head;
    printf("包裹ID\t\t取件码\t\t\t状态\t\t更多信息\n");
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            if (current->status == PENDING_PICKUP) {
                printf("%d\t\t%s\t\t%s\t\t[%d]\n", current->package_id,
                       current->pickup_code,
                       packageSatatusToString(current->status),
                       current->package_id);
            } else {
                printf("%d\t\t%s\t\t\t%s\t\t[%d]\n", current->package_id, "- - -",

                       packageSatatusToString(current->status),
                       current->package_id);
            }

            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("----------------------------------------\n");
        printf("未找到该用户的包裹\n");
        printf("----------------------------------------\n");
        return;
    }
    printf("----------------------------------------\n");
    printf("输入包裹ID以查看更多详细信息（输入0跳过）：");
    int package_id;
    package_id = getValidatedIntegerInput(1000, 9999, 1);
    if (package_id != 0) {
        current = system->head;
        while (current != NULL) {
            if (current->package_id == package_id &&
                strcmp(current->username, username) == 0) {
                char store_time_str[30] = "";
                char pickup_time_str[30] = "";
                char sent_time_str[30] = "";
                struct tm* timeinfo;

                timeinfo = localtime(&current->store_time);
                strftime(store_time_str, sizeof(store_time_str),
                         "%m-%d %H:%M:%S", timeinfo);

                if (current->pickup_time > 0) {
                    timeinfo = localtime(&current->pickup_time);
                    strftime(pickup_time_str, sizeof(pickup_time_str),
                             "%m-%d %H:%M:%S", timeinfo);
                } else {
                    strcpy(pickup_time_str, "-");
                }

                printf("\n包裹详细信息:\n");
                printf("包裹ID: %d\n", current->package_id);
                printf("所有者: %s\n", current->username);
                printf("状态: %s\n", packageSatatusToString(current->status));
                printf("描述: %s\n", current->description);

                printf("取件码: %s\n", current->pickup_code);
                printf("位置: %d号货架, %d层\n", current->shelf_number,
                       current->layer_number);
                if (current->status == PENDING_DELIVERY) {
                    printf("收件人: %s\n", current->recipient);
                    printf("收件人地址: %s\n", current->recipientAdress);
                    printf("配送费用: %.2f\n", current->delivery_fee);
                }

                printf("存储时间: %s\n", store_time_str);
                if (current->status == PICKED_UP) {
                    printf("取件时间: %s\n", pickup_time_str);
                } else if (current->status == DELIVERED) {
                    if (current->sent_time > 0) {
                        timeinfo = localtime(&current->sent_time);
                        strftime(sent_time_str, sizeof(sent_time_str),
                                 "%m-%d %H:%M:%S", timeinfo);
                        printf("寄出时间: %s\n", sent_time_str);
                    }
                }
                printf("----------------------------------------\n");

                break;
            }
            current = current->next;
        }
        if (current == NULL) {
            printf("未找到包裹\n");
        }
    }
}

// 取件包裹
int pickupPackage(PackageSystem* system, int package_id) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return 0;
    }

    PackageNode* current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            if (current->status == PENDING_PICKUP) {
                current->status = PICKED_UP;
                current->pickup_time = time(NULL);  // 记录取件时间
                printf("包裹取件成功\n");

                // 取件后自动保存到文件
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("保存包裹数据失败\n");
                }

                return 1;
            } else {
                printf("此包裹无法取件\n");
                return 0;
            }
        }
        current = current->next;
    }
    printf("未找到包裹\n");
    return 0;
}

// 清除所有包裹数据
// 显示所有包裹
void displayAllPackages(PackageSystem* system) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    printf("\n所有包裹信息:\n");
    printf("包裹ID\t\t状态\t\t\t更多信息\n");

    PackageNode* current = system->head;
    while (current != NULL) {
        printf("%d\t\t%s\t\t[%d]\n", current->package_id,
               packageSatatusToString(current->status), current->package_id);
        current = current->next;
    }

    printf("\n输入包裹ID以查看更多详细信息（输入0返回）：");
    int package_id;
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id != 0) {
        current = system->head;
        while (current != NULL) {
            if (current->package_id == package_id) {
                char store_time_str[30] = "";
                char pickup_time_str[30] = "";
                char sent_time_str[30] = "";
                struct tm* timeinfo;

                timeinfo = localtime(&current->store_time);
                strftime(store_time_str, sizeof(store_time_str),
                         "%m-%d %H:%M:%S", timeinfo);

                if (current->pickup_time > 0) {
                    timeinfo = localtime(&current->pickup_time);
                    strftime(pickup_time_str, sizeof(pickup_time_str),
                             "%m-%d %H:%M:%S", timeinfo);
                } else {
                    strcpy(pickup_time_str, "-");
                }

                printf("\n包裹详细信息:\n");
                printf("包裹ID: %d\n", current->package_id);
                printf("所属用户: %s\n", current->username);
                printf("状态: %s\n", packageSatatusToString(current->status));
                printf("描述: %s\n", current->description);

                printf("取件码: %s\n", current->pickup_code);
                printf("位置: 货架 %d, 层 %d\n", current->shelf_number,
                       current->layer_number);
                if (current->status == PENDING_DELIVERY) {
                    printf("收件人: %s\n", current->recipient);
                    printf("配送费用: %.2f\n", current->delivery_fee);
                }

                printf("存储时间: %s\n", store_time_str);

                if (current->status == PICKED_UP) {
                    printf("取件时间: %s\n", pickup_time_str);

                } else if (current->status == DELIVERED) {
                    if (current->sent_time > 0) {
                        timeinfo = localtime(&current->sent_time);
                        strftime(sent_time_str, sizeof(sent_time_str),
                                 "%m-%d %H:%M:%S", timeinfo);
                        printf("寄出时间: %s\n", sent_time_str);
                    }
                }
                break;
            }
            current = current->next;
        }
        if (current == NULL) {
            printf("未找到包裹\n");
        }
    }
}

void clearAllPackages(PackageSystem* system) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    PackageNode* current = system->head;
    PackageNode* next;

    // 释放所有节点的内存
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // 重置系统状态
    system->head = NULL;
    system->package_count = 0;

    // 清空数据文件
    FILE* file = fopen(PACKAGE_FILE, "wt");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("清除包裹数据文件失败\n");
    }
}

char* packageSatatusToString(PackageStatus status) {
    switch (status) {
        case PICKED_UP:
            return (char*)"已取出";
            break;
        case PENDING_DELIVERY:
            return (char*)"待寄出";
            break;
        case DELIVERED:
            return (char*)"已寄出";
            break;
        case PENDING_PICKUP:
            return (char*)"待取件";
            break;
        case ABNORMAL:
            return (char*)"异常";
            break;
        case REJECTED:
            return (char*)"拒收";
            break;
        default:
            return (char*)"未知";
            break;
    }
}
int generatePackageId(PackageSystem* system) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return 0;
    }

    // 使用当前时间作为随机数种子
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand((unsigned int)time(NULL));
        seed_initialized = 1;
    }

    int new_id;
    int id_found = 0;
    PackageNode* current;

    // 生成随机ID并确保不重复
    while (!id_found) {
        // 生成1000-9999范围内的随机数
        new_id = 1000 + (rand() % 9000);
        int id_exists = 0;
        current = system->head;

        // 检查ID是否已存在
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
                printf("\n可选用户列表：\n");
                UserNode* user = user_system->head;
                while (user != NULL) {
                    printf("%s\n", user->username);
                    user = user->next;
                }
                printf("请输入包裹所有者的用户名: ");
                getValidatedStringInput(username, MAX_USERNAME_LENGTH);

                // 验证用户名是否存在
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
                    printf("用户名不存在\n");
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

    printf("请输入包裹描述: ");
    getValidatedStringInput(description, MAX_NAME_LENGTH);

    printf("请选择包裹大小 (0:小件, 1:中件, 2:大件): ");
    size_choice = getValidatedIntegerInput(0, 2, 1);

    printf("请输入包裹重量(kg): ");
    weight_kg = getValidatedFloatInput(0.1f, 100.0f);
    // 为包裹重量也设置了上限100
    printf("请选择紧急程度 (0:普通, 1:加急): ");
    urgency_choice = getValidatedIntegerInput(0, 1, 1);

    printf("请选择包裹类型 (0:普通包裹, 1:贵重物品, 2:易碎物品): ");
    type_choice = getValidatedIntegerInput(0, 2, 1);

    printf("请选择存储要求 (0:常温, 1:冷藏): ");
    storage_choice = getValidatedIntegerInput(0, 1, 1);

    char recipient[MAX_USERNAME_LENGTH];
    float delivery_fee = 0.0f;
    char adress[MAX_ADDRESS_LENGTH];
    if ((user_type != USER_ADMIN) && (user_type != USER_COURIER)) {
        printf("请输入收件人姓名: ");
        getValidatedStringInput(recipient, MAX_USERNAME_LENGTH);
        printf("请输入收件人地址: ");
        getValidatedStringInput(adress, MAX_ADDRESS_LENGTH);

        delivery_fee =
            calculateDeliveryFee(weight_kg, size_choice, urgency_choice,
                                 type_choice, storage_choice, user_type);
        pauseAndClearConsole(0);
        printf("----------------------------------------\n");
        printf("配送费用明细:\n");
        printf("----------------------------------------\n");
        printf("基础运费 (%.2f 元/kg):\t%.2f 元\n", 0.8f, weight_kg * 0.8f);

        float base_fee = weight_kg * 0.8f;
        float size_fee = 0.0f, urgency_fee = 0.0f, type_fee = 0.0f,
              storage_fee = 0.0f;

        // Size-based fee
        switch (size_choice) {
            case 1:
                size_fee = base_fee * 0.5f;
                printf("中件包裹附加费 (50%%):\t\t%.2f 元\n", size_fee);
                break;
            case 2:
                size_fee = base_fee * 1.0f;
                printf("大件包裹附加费 (100%%):\t\t%.2f 元\n", size_fee);
                break;
        }

        // Urgency fee
        if (urgency_choice == 1) {
            urgency_fee = (base_fee + size_fee) * 0.5f;
            printf("加急服务费 (50%%):\t%.2f 元\n", urgency_fee);
        }

        // Package type fee
        if (type_choice == 1 || type_choice == 2) {
            type_fee = (base_fee + size_fee + urgency_fee) * 0.5f;
            printf("%s附加费 (50%%):\t%.2f 元\n",
                   type_choice == 1 ? "贵重物品" : "易碎物品", type_fee);
        }

        // Storage requirement fee
        if (storage_choice == 1) {
            storage_fee = (base_fee + size_fee + urgency_fee + type_fee) * 0.5f;
            printf("冷藏服务费 (50%%):\t%.2f 元\n", storage_fee);
        }

        float subtotal =
            base_fee + size_fee + urgency_fee + type_fee + storage_fee;
        printf("----------------------------------------\n");
        printf("小计:\t\t\t%.2f 元\n", subtotal);

        // User discount
        float discount = 0.0f;
        if (user_type != USER_NORMAL && user_type != USER_ADMIN) {
            discount = subtotal * (1.0f - (user_type == USER_STUDENT   ? 0.8f
                                           : user_type == USER_TEACHER ? 0.7f
                                           : user_type == USER_VIP     ? 0.6f
                                                                       : 1.0f));
            printf("用户折扣 (%s):\t-%.2f 元\n",
                   user_type == USER_STUDENT   ? "八折"
                   : user_type == USER_TEACHER ? "七折"
                   : user_type == USER_VIP     ? "六折"
                                               : "无折扣",
                   discount);
        }

        printf("----------------------------------------\n");
        printf("总金额:\t\t%.2f 元\n", delivery_fee);
        printf("----------------------------------------\n");

        char confirm;
        printf("----------------------------------------\n");
        printf("确认支付并存储包裹？(y/n): ");
        confirm = getValidatedCharInput("YNyn");
        if (confirm != 'y' && confirm != 'Y') {
            printf("包裹存储已取消\n");
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
        printf("包裹添加成功！\n");
        printf("包裹ID: %d\n", result);
        if (user_type == USER_ADMIN || user_type == USER_COURIER) {
            printf("状态: 待取件\n");
        } else {
            printf("状态: 待寄出\n");
        }
        // 默认快递和管理员存入的都是待取出，其他用户是待寄出
        printf("----------------------------------------\n");
        if (user_type != USER_ADMIN && user_type != USER_COURIER) {
            printf("发件人: %s\n", username);
            printf("收件人: %s\n", recipient);
            printf("地址: %s\n", adress);
            printf("快递费用: %.2f 元\n", delivery_fee);
            printf("----------------------------------------\n");
        }
    } else {
        printf("添加包裹失败\n");
    }
}
void handleQueryPackages(PackageSystem* system, UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];

    // 获取当前登录用户的用户名和类型

    UserNode* current = user_system->head;
    UserType user_type;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            user_type = current->type;
            getSpecificUser(user_system, username, user_type,
                            current->username);
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
            getSpecificUser(user_system, username, user_type,
                            current->username);
            break;
        }
        current = current->next;
    }

    queryPackagesByUsername(system, username);

    int package_id;
    printf("\n请输入要取件的包裹ID（输入0返回）：");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id != 0) {
        pickupPackage(system, package_id);
    }
}

void handleDeliverPackage(PackageSystem* system) {
    int package_id;
    int found_pending = 0;

    printf("\n待寄出/被拒绝的包裹列表:\n");
    printf("包裹ID\t描述\t\t收件人\t\t配送费用\n");

    PackageNode* current = system->head;
    while (current != NULL) {
        // 检查是否处于待配送状态
        if (current->status == PENDING_DELIVERY ||
            current->status == REJECTED) {  // 待配送状态
            printf("%d\t%-16s\t%-16s\t%.2f\n", current->package_id,
                   current->description, current->recipient,
                   current->delivery_fee);
            found_pending = 1;
        }
        current = current->next;
    }

    if (!found_pending) {
        printf("没有待寄出的包裹\n");
        return;
    }

    printf("\n请输入要寄出的包裹ID: ");
    package_id = getValidatedIntegerInput(1000, 9999, 0);

    current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            // 检查包裹是否处于待配送状态并更新为已配送
            if (current->status == PENDING_DELIVERY) {
                current->status = DELIVERED;      // 更新为已配送状态
                current->sent_time = time(NULL);  // 记录配送时间
                printf("包裹已成功寄出\n");

                // 配送后自动保存到文件
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("保存包裹数据失败\n");
                }
                return;
            } else {
                printf("此包裹不处于待寄出状态\n");
                return;
            }
        }
        current = current->next;
    }
    printf("未找到包裹\n");
}

void handlePackageStatistics(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    int total_packages = 0;
    int pending_pickup = 0;
    int pending_delivery = 0;
    int abnormal = 0;
    int rejected = 0;

    PackageNode* package = system->head;
    while (package != NULL) {
        switch (package->status) {
            case PENDING_PICKUP:
                pending_pickup++;
                total_packages++;
                break;
            case PENDING_DELIVERY:
                pending_delivery++;
                total_packages++;
                break;
            case ABNORMAL:
                abnormal++;
                total_packages++;
                break;
            case REJECTED:
                rejected++;
                total_packages++;
                break;
            default:
                break;
        }
        package = package->next;
    }

    float usage_rate = (float)total_packages / MAX_PACKAGES * 100;
    float free_rate = 100.0f - usage_rate;
    printf("----------------------------------------\n");
    printf("包裹系统统计:\n");
    printf("总包裹数:\t%d\n", total_packages);
    printf("系统使用率:\t%.2f%%\n", usage_rate);
    printf("系统空闲率:\t%.2f%%\n", free_rate);
    printf("----------------------------------------\n");
    printf("包裹状态统计:\n");
    printf("待取件:\t\t%d\n", pending_pickup);
    printf("待寄出:\t\t%d\n", pending_delivery);
    printf("异常状态:\t%d\n", abnormal);
    printf("被拒绝的包裹:\t%d\n", rejected);
    printf("----------------------------------------\n");
}
/**
 * 系统数据清空功能
 * @param system 包裹管理系统指针
 * @param user_system 用户系统指针
 * @param
 */
void handleClearSystemData(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    char confirm;
    printf("警告: 此操作将清除所有用户和包裹以及反馈数据。继续吗？(y/n): ");
    confirm = getValidatedCharInput("YNyn");

    if (confirm == 'y' || confirm == 'Y') {
        clearAllPackages(system);
        clearAllUsers(user_system);
        FeedbackSystem* feedback_system = loadFeedbackFromFile();
        clearAllFeedback(feedback_system);
        printf("系统数据已清除\n");
    } else {
        printf("操作已取消\n");
    }
}

/**
 * 处理标记异常包裹功能
 * @param system 包裹管理系统指针
 * @param user_system 用户系统指针
 */
void handleMarkAbnormalPackage(PackageSystem* system, UserSystem* user_system) {
    if (system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    // 显示用户列表
    printf("\n可选用户列表：\n");
    UserNode* user = user_system->head;
    while (user != NULL) {
        printf("%s\n", user->username);
        user = user->next;
    }

    // 选择用户
    char username[MAX_USERNAME_LENGTH];
    printf("请输入要检查包裹的用户名：");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);

    // 验证用户是否存在
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
        printf("用户名不存在\n");
        return;
    }

    // 显示该用户的包裹信息
    printf("\n用户 %s 的包裹：\n", username);
    printf("包裹ID\t描述\t\t状态\n");
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
        printf("未找到该用户的包裹\n");
        return;
    }

    // 选择要标记的包裹
    int package_id;
    printf("\n请输入要标记为异常的包裹ID（输入0取消）：");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id == 0) {
        printf("操作已取消\n");
        return;
    }

    // 更新包裹状态
    current = system->head;
    while (current != NULL) {
        if (current->package_id == package_id) {
            if (strcmp(current->username, username) == 0) {
                current->status = ABNORMAL;
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("保存包裹数据失败\n");
                } else {
                    printf("包裹已成功标记为异常状态\n");
                }
                return;
            } else {
                printf("该包裹不属于选定用户\n");
                return;
            }
        }
        current = current->next;
    }
    printf("未找到包裹\n");
}
// 处理包裹拒收
// 允许用户拒收包裹，管理员/快递员和普通用户有不同的显示逻辑
// 参数：system - 包裹管理系统指针，user_system - 用户管理系统指针
void handleRejectPackage(PackageSystem* system, UserSystem* user_system) {
    // 检查系统初始化状态
    if (system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    // 获取当前用户信息

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
    getSpecificUser(user_system, target_username, current_user_type,
                    current_username);
    // 显示选定用户的包裹信息
    printf("\n----------------------------------------\n");
    printf("用户 %s 的包裹列表:\n", target_username);
    printf("----------------------------------------\n");
    int found = 0;
    PackageNode* package = system->head;
    while (package != NULL) {
        if (strcmp(package->username, target_username) == 0) {
            printf("包裹编号: %d\n", package->package_id);
            printf("包裹描述: %s\n", package->description);
            printf("包裹状态: %s\n", packageSatatusToString(package->status));
            printf("----------------------------------------\n");
            found = 1;
        }
        package = package->next;
    }

    if (!found) {
        printf("----------------------------------------\n");
        printf("该用户没有包裹\n");
        printf("----------------------------------------\n");
        return;
    }

    // 处理包裹拒收操作
    int package_id;
    printf("请输入要拒收的包裹编号 (输入0取消): ");
    package_id = getValidatedIntegerInput(1000, 9999, 1);

    if (package_id == 0) {
        printf("----------------------------------------\n");
        printf("操作已取消\n");
        printf("----------------------------------------\n");
        return;
    }

    // 查找并更新包裹状态
    package = system->head;
    while (package != NULL) {
        if (package->package_id == package_id) {
            // 验证包裹所属权
            if (strcmp(package->username, target_username) != 0) {
                printf("该包裹不属于选定用户\n");
                return;
            }

            // 检查包裹状态是否可以拒收
            if (package->status == PENDING_PICKUP ||
                package->status == ABNORMAL) {
                package->status = REJECTED;
                if (!savePackagesToFile(system, PACKAGE_FILE)) {
                    printf("保存包裹数据失败\n");
                } else {
                    printf("----------------------------------------\n");
                    printf("包裹已成功拒收\n");
                    printf("包裹状态: %s\n",
                           packageSatatusToString(package->status));
                    printf("将由快递员寄回原地址\n");
                    printf("----------------------------------------\n");
                }
                return;
            } else {
                printf("----------------------------------------\n");
                printf("当前状态下的包裹无法拒收\n");
                printf("----------------------------------------\n");
                return;
            }
        }
        package = package->next;
    }
    printf("----------------------------------------\n");
    printf("未找到该包裹\n");
    printf("----------------------------------------\n");
}

int generatePackageLocation(PackageSystem* system,
                            int* shelf_number,
                            int* layer_number) {
    // 参数有效性检查
    if (system == NULL || shelf_number == NULL || layer_number == NULL) {
        return 0;
    }

    // 使用当前时间作为随机数种子
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand((unsigned int)time(NULL));
        seed_initialized = 1;
    }

    int attempts = 0;
    const int MAX_ATTEMPTS = 50;  // 由于位置组合较少，降低尝试次数

    do {
        // 随机生成货架号(1-5)和层数(0-1)
        *shelf_number = (rand() % SHELF_COUNT) + 1;
        *layer_number = rand() % LAYER_COUNT;

        // 检查位置是否已被占用或已满
        int location_occupied = 0;
        int packages_in_location = 0;
        PackageNode* current = system->head;

        while (current != NULL) {
            // 只检查未取走和未送达的包裹
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

        // 如果位置未被占满，返回成功
        if (!location_occupied) {
            printf(
                "已为包裹分配位置：货架 %d 第 %d 层（当前该位置已有 %d "
                "个包裹）\n",
                *shelf_number, *layer_number + 1, packages_in_location);
            return 1;
        }

        attempts++;
    } while (attempts < MAX_ATTEMPTS);

    // 如果尝试次数达到上限仍未找到可用位置，返回失败
    printf("警告：无法找到可用的存储位置，所有位置可能已满\n");
    return 0;
}