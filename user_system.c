#include "all_h_files.h"
#define ADMIN_KEY "admin"
// 初始化用户管理系统
// 分配内存并初始化系统的基本数据结构
// 返回值：成功返回系统指针，失败返回NULL
UserSystem* initUserSystem() {
    // 为系统结构体分配内存
    UserSystem* system = (UserSystem*)malloc(sizeof(UserSystem));
    if (system == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    // 初始化系统状态
    system->head = NULL;       // 用户链表头指针初始化为空
    system->user_count = 0;    // 用户数量初始化为0
    system->is_login = false;  // 当前登录用户is_login状态初始化为false
    return system;
}

// 从文件加载用户数据
// 读取二进制文件中的用户信息，重建用户链表
// 参数：system - 系统指针，filename - 数据文件名
// 返回值：成功返回1，失败返回0
int loadUsersFromFile(UserSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }
    // 以文本模式打开文件
    FILE* file = fopen(filename, "rt");
    if (file == NULL) {
        return 0;
    }
    // 注意文件第一个位置存的用户数量
    //  读取用户总数
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // 读取用户数据并重建链表
    UserNode temp;
    system->head = NULL;
    system->user_count = 0;

    // 循环读取每个用户的数据
    for (int i = 0; i < count; i++) {
        // 读取一个用户节点的数据
        if (fread(&temp, sizeof(UserNode), 1, file) != 1) {
            // 读取失败
            fclose(file);
            return 0;
        }

        // 为新节点分配内存
        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        if (new_node == NULL) {
            fclose(file);
            return 0;
        }
        // 注意区分temp是一个实实在在的节点，不是指针
        //  复制数据到新节点
        *new_node = temp;
        new_node->next = NULL;

        // 将新节点添加到链表末尾
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

// 保存用户数据到文件
// 将当前用户链表中的所有用户信息写入二进制文件
// 参数：system - 系统指针，filename - 数据文件名
// 返回值：成功返回1，失败返回0
int saveUsersToFile(UserSystem* system, const char* filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }

    // 以文本写模式打开文件
    FILE* file = fopen(filename, "wt");
    if (file == NULL) {
        return 0;
    }

    // 写入用户总数
    if (fwrite(&system->user_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }

    // 逐个写入用户数据
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

// 注册新用户
// 创建新用户并添加到系统中，同时保存到文件
// 参数：system - 系统指针，username - 用户名，password - 密码，type - 用户类型
// 返回值：成功返回用户名，失败返回NULL
char* registerUser(UserSystem* system,
                   const char* username,
                   const char* password,
                   UserType type) {
    if (system == NULL || username == NULL || password == NULL) {
        return NULL;
    }
    // 检查用户名是否已存在，并将指针指向链表末尾
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("用户名已存在！\n");
            return NULL;
        }
        current = current->next;
    }
    // 创建新用户节点
    UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
    if (new_node == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    strncpy(new_node->username, username, MAX_USERNAME_LENGTH - 1);
    new_node->username[MAX_USERNAME_LENGTH - 1] = '\0';
    strncpy(new_node->password, password, MAX_PASSWORD_LENGTH - 1);
    new_node->password[MAX_PASSWORD_LENGTH - 1] = '\0';
    new_node->type = type;
    new_node->friend_count = 0;
    new_node->friends[0][0] = '\0';  // 初始化 friends[0]
    // 为注册的用户分配随机的邀请码
    strcpy(new_node->invateCode, generateInviteCode());
    new_node->next = NULL;
    // 将新节点添加到链表末尾
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

    // 注册成功后立即保存数据保证数据不丢失
    if (!saveUsersToFile(system, USER_FILE)) {
        printf("保存用户数据失败！\n");
    }
    return new_node->username;
}

// 用户登录
// 验证用户名和密码，设置当前登录用户
// 参数：system - 系统指针，username - 用户名，password - 密码
// 返回值：成功返回1，失败返回0
int loginUser(UserSystem* system, const char* username, const char* password) {
    if (system == NULL || username == NULL || password == NULL) {
        return 0;
    }

    // 遍历用户链表查找匹配的用户名和密码
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 &&
            strcmp(current->password, password) == 0) {
            strcpy(system->current_username, username);
            system->is_login = true;
            pauseAndClearConsole(0);
            printf("----------------------------------------\n");
            printf("登录成功！用户类型：%s\n", USER_TYPE_NAMES[current->type]);
            return 1;
        }
        current = current->next;
    }
    printf("用户名或密码无效\n");
    return 0;
}
// 用户登出
// 清除当前登录用户的状态
void logoutUser(UserSystem* system) {
    if (system != NULL) {
        system->is_login = false;
    }
    if (system->is_login == false) {
        printf("已退出登录\n");
        printf("----------------------------------------\n");
    }
}

// 清除所有用户数据
// 释放链表内存并重置系统状态
void clearAllUsers(UserSystem* system) {
    if (system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    // 释放所有节点的内存
    UserNode* current = system->head;
    UserNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // 重置系统状态
    system->head = NULL;
    system->user_count = 0;
    system->is_login = false;

    // 清空数据文件
    FILE* file = fopen(USER_FILE, "wt");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("清除用户数据文件失败\n");
    }
}

// 修改用户密码
// 验证原密码并更新为新密码
// 参数：system - 系统指针，username - 用户名
// 返回值：成功返回1，失败返回0
int changeUserPassword(UserSystem* system, const char* username) {
    if (system == NULL || username == NULL) {
        return 0;
    }

    // 查找用户
    UserNode* current = system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            char old_password[MAX_PASSWORD_LENGTH];
            char new_password[MAX_PASSWORD_LENGTH];
            char confirm_password[MAX_PASSWORD_LENGTH];
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;  // 最大尝试次数

            // 验证原密码，最多允许3次尝试
            while (attempts < MAX_ATTEMPTS) {
                printf("\n请输入原密码 (还剩 %d 次尝试机会): ",
                       MAX_ATTEMPTS - attempts);
                getValidatedStringInput(old_password, MAX_PASSWORD_LENGTH);

                if (strcmp(current->password, old_password) == 0) {
                    // 原密码验证成功，继续修改密码流程
                    printf("\n原密码验证成功！\n");

                    // 获取新密码
                    printf("\n请输入新密码: ");
                    getValidatedStringInput(new_password, MAX_PASSWORD_LENGTH);

                    printf("请确认新密码: ");
                    getValidatedStringInput(confirm_password,
                                            MAX_PASSWORD_LENGTH);

                    // 验证两次输入的新密码是否一致
                    if (strcmp(new_password, confirm_password) != 0) {
                        printf("\n两次输入的新密码不一致，密码修改失败\n");
                        return 0;
                    }

                    // 更新密码
                    strncpy(current->password, new_password,
                            MAX_PASSWORD_LENGTH - 1);
                    current->password[MAX_PASSWORD_LENGTH - 1] = '\0';

                    // 保存更改到文件
                    if (!saveUsersToFile(system, USER_FILE)) {
                        printf("保存密码修改失败\n");
                        return 0;
                    }

                    return 1;
                } else {
                    attempts++;
                    if (attempts < MAX_ATTEMPTS) {
                        printf("\n原密码错误！还剩 %d 次尝试机会\n",
                               MAX_ATTEMPTS - attempts);
                    } else {
                        printf("\n连续 %d 次密码验证失败，账户将被登出\n",
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

    printf("未找到用户\n");
    return 0;
}

// 处理修改密码的用户交互
// 使用安全的输入验证函数获取用户输入
void handleChangePassword(UserSystem* system) {
    if (system == NULL || system->is_login == false) {
        printf("请先登录\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    strncpy(username, system->current_username, MAX_USERNAME_LENGTH);
    if (username == NULL) {
        printf("获取用户信息失败\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("修改密码\n");
    printf("----------------------------------------\n");

    // 调用修改密码函数
    if (changeUserPassword(system, username)) {
        printf("----------------------------------------\n");
        printf("密码修改成功，下次登录请使用新密码\n");
        printf("----------------------------------------\n");
    } else {
        if (system->is_login == false) {
            printf("----------------------------------------\n");
            printf("由于多次密码验证失败，账户已被登出\n");
            printf("----------------------------------------\n");
        } else {
            printf("----------------------------------------\n");
            printf("密码修改失败\n");
            printf("----------------------------------------\n");
        }
    }
}

// 用户注销
// 从系统中删除当前登录用户的账户
// 参数：system - 用户管理系统指针
// 返回值：成功返回1，失败返回0
int handleDeleteUserAccount(UserSystem* user_system,
                            PackageSystem* package_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return 0;
    }

    // 获取当前用户信息
    UserNode* current = user_system->head;
    UserNode* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            // 验证用户身份
            char password[MAX_PASSWORD_LENGTH];
            printf("\n----------------------------------------\n");
            printf("账户注销\n");
            printf("----------------------------------------\n");
            int attempts = 0;
            const int MAX_ATTEMPTS = 3;  // 最大尝试次数
            char old_password[MAX_PASSWORD_LENGTH];
            // 验证密码，最多允许3次尝试
            while (attempts < MAX_ATTEMPTS) {
                printf("\n请输入密码 (还剩 %d 次尝试机会): ",
                       MAX_ATTEMPTS - attempts);
                getValidatedStringInput(old_password, MAX_PASSWORD_LENGTH);

                if (strcmp(current->password, old_password) == 0) {
                    // 原密码验证成功，继续注销账户
                    printf("\n当前账户密码验证成功！\n");
                    // 再次确认
                    char confirm;
                    printf("\n警告：此操作将永久删除您的账户，且不可恢复！\n");
                    printf("\n警告：包裹存取记录，好友信息也将被删除！\n");
                    printf("确认注销账户？(y/n): ");
                    confirm = getValidatedCharInput("YNyn");

                    if (confirm != 'y' && confirm != 'Y') {
                        printf("操作已取消\n");
                        printf("----------------------------------------\n");
                        return 0;
                    }
                    if (!deleteUser(user_system, package_system,
                                   user_system->current_username)){
                        return 0;
                                   }

                        // 保存更改到文件
                        if (!saveUsersToFile(user_system, USER_FILE)) {
                            printf("保存用户数据失败\n");
                            printf(
                                "----------------------------------------\n");
                            return 0;
                        }
                        if(!savePackagesToFile(package_system, PACKAGE_FILE)){
                            printf("保存包裹数据失败\n");
                            printf(
                                "----------------------------------------\n");
                            return 0; 
                        }
                    printf("账户已成功注销\n");
                    printf("----------------------------------------\n");
                    return 1;
                } else {
                    attempts++;
                    if (attempts < MAX_ATTEMPTS) {
                        printf("\n密码错误！还剩 %d 次尝试机会\n",
                               MAX_ATTEMPTS - attempts);
                    } else {
                        printf("\n连续 %d 次密码验证失败，账户将被登出\n",
                               MAX_ATTEMPTS);
                        logoutUser(user_system);
                        return 0;
                    }
                }
            }
        }
        prev = current;
        current = current->next;
    }

    printf("未找到用户信息\n");
    printf("----------------------------------------\n");
    return 0;
}
int deleteUser(UserSystem* user_system, PackageSystem* package_system, const char* username) {
    int reminder = 0;
    PackageNode* curr_package = package_system->head;
    PackageNode* pre_package = NULL; // 初始化为 NULL

    while (curr_package) {
        if (strcmp(curr_package->username, username) == 0) {
            if (curr_package->status != PICKED_BY_OTHER && curr_package->status != PICKED_UP && curr_package->status != DELIVERED && curr_package->status != CLEANED) {
                reminder = 1;
                printf("提示：当前账户中仍然存在需要处理的包裹\n请确保处理完包裹后执行注销操作!\n");
                return 0;
            } else {
                // 执行删除包裹结点的操作
                if (curr_package == package_system->head) {
                    package_system->head = curr_package->next;
                } else {
                    pre_package->next = curr_package->next;
                }
                PackageNode* temp = curr_package;
                curr_package = curr_package->next;
                free(temp);
                package_system->package_count--;
                continue; // 跳过 pre_package 更新
            }
        }
        pre_package = curr_package;
        curr_package = curr_package->next;
    }

    UserNode* curr_user = user_system->head;
    UserNode* prev = NULL;
    while (curr_user != NULL) {
        // 找到用户结点
        if (strcmp(curr_user->username, username) == 0) {
            // 查找当前用户的好友执行双向删除
            for (int i = 0; i < curr_user->friend_count; ++i) {
                deleteFriend(user_system, curr_user->friends[i]);
            }
            break;
        }
        prev = curr_user;
        curr_user = curr_user->next;
    }
    if (curr_user == NULL) {
        printf("用户登录状态异常\n");
        return 0;
    }
    // 删除该用户结点
    if (prev == NULL) {
        user_system->head = curr_user->next;
    } else {
        prev->next = curr_user->next;
    }
    free(curr_user);
    user_system->user_count--;
    user_system->is_login = false;
    return 1;
}
// 处理用户注册
// 引导用户输入注册信息，包括用户名、密码和用户类型
// 对不同类型的用户进行相应的验证
// 参数：user_system - 用户管理系统指针
void handleUserRegister(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int type_choice;
    // 获取基本注册信息
    printf("请输入用户名: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);

    UserNode* current = user_system->head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            printf("用户名已存在！\n");
            printf("注册失败，请重试\n");
            return;
        }
        current = current->next;
    }

    printf("请输入密码: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    // 显示用户类型选项
    printf("----------------------------------------\n");
    printf("请选择用户类型:\n");
    printf("0. 学生用户\n");
    printf("1. 教师用户\n");
    printf("2. VIP用户\n");
    printf("3. 普通用户\n");
    printf("4. 管理员用户\n");
    printf("----------------------------------------\n");
    printf("请选择 (0-4): ");
    type_choice = getValidatedIntegerInput(0, 4, 1);

    // 根据不同用户类型进行特殊验证
    if (type_choice == USER_STUDENT || type_choice == USER_TEACHER) {
        // 学生和教师需要验证教育邮箱
        char email[100];
        printf("请输入吉大邮箱 (格式: xxx@jlu.edu.cn): ");
        getValidatedStringInput(email, 100);

        char* at_pos = strstr(email, "@jlu.edu.cn");
        if (at_pos == NULL || strcmp(at_pos, "@jlu.edu.cn") != 0) {
            printf("邮箱格式无效，注册失败\n");
            return;
        }
        printf("邮箱验证成功！\n");
    } else if (type_choice == USER_VIP) {
        // VIP用户需要确认年费支付
        char pay_confirm;
        printf("VIP用户需要支付年费200元，确认支付吗？(y/n): ");
        pay_confirm = getValidatedCharInput("YNyn");

        if (pay_confirm != 'y' && pay_confirm != 'Y') {
            printf("支付未完成，注册失败\n");
            return;
        }
        printf("支付成功！\n");
    } else if (type_choice == USER_ADMIN) {
        // 管理员需要验证管理员密钥
        char admin_key[50];
        printf("请输入管理员密钥: ");
        getValidatedStringInput(admin_key, 50);
        if (strcmp(admin_key, ADMIN_KEY) != 0) {
            printf("管理员密钥无效，注册失败\n");
            return;
        }
    }

    // 执行用户注册
    if (registerUser(user_system, username, password, (UserType)type_choice)) {
        printf("----------------------------------------\n");
        printf("注册成功！用户类型: %s\n", USER_TYPE_NAMES[type_choice]);
    } else {
        printf("注册失败，请重试\n");
    }
}

// 处理用户登录
// 验证用户登录信息，包括用户名和密码
// 参数：user_system - 用户管理系统指针
void handleUserLogin(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("请输入用户名: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);
    printf("请输入密码: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    if (loginUser(user_system, username, password)) {
    } else {
        printf("登录失败，请检查用户名和密码\n");
        pauseAndClearConsole(1);
    }
}

// 处理快递员注册
// 仅供管理员使用，用于创建新的快递员账户
// 参数：user_system - 用户管理系统指针
void handleCourierRegister(UserSystem* user_system) {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("请输入快递员用户名: ");
    getValidatedStringInput(username, MAX_USERNAME_LENGTH);
    printf("请输入快递员密码: ");
    getValidatedStringInput(password, MAX_PASSWORD_LENGTH);

    if (registerUser(user_system, username, password, USER_COURIER)) {
        printf("快递员账户注册成功！\n");
    } else {
        printf("快递员账户注册失败，请重试\n");
    }
}
