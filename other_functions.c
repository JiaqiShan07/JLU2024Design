#include "all_h_files.h"
void pauseAndClearConsole(int getchar_num) {
    if (getchar_num != 0) {
        printf("按Enter键继续...");
        for (int i = 0; i < getchar_num; i++) {
            getchar();
        }
    }
    system("cls");
}
float calculateDeliveryFee(float weight_kg,
                           int size_choice,
                           int urgency_choice,
                           int type_choice,
                           int storage_choice,
                           UserType user_type) {
    float delivery_fee = weight_kg * 0.8f;

    // 根据包裹大小调整费用
    switch (size_choice) {
        case 1:  // 中件
            delivery_fee *= 1.5f;
            break;
        case 2:  // 大件
            delivery_fee *= 2.0f;
            break;
        default:  // 小件，不额外收费
            break;
    }

    // 根据紧急程度调整费用
    if (urgency_choice == 1) {  // 加急服务
        delivery_fee *= 1.5f;
    }

    // 根据包裹类型调整费用
    if (type_choice == 1 || type_choice == 2) {  // 贵重物品或易碎物品
        delivery_fee *= 1.5f;
    }

    // 根据存储要求调整费用
    if (storage_choice == 1) {  // 冷藏服务
        delivery_fee *= 1.5f;
    }

    // 应用用户类型折扣
    switch (user_type) {
        case USER_STUDENT:
            delivery_fee *= 0.8f;  // 学生八折
            break;
        case USER_TEACHER:
            delivery_fee *= 0.7f;  // 教师七折
            break;
        case USER_VIP:
            delivery_fee *= 0.6f;  // VIP用户六折
            break;
        default:  // 普通用户和管理员无折扣
            break;
    }

    return delivery_fee;
}
// 这是一个辅助函数，用于列出用户列表并且查询指定用户
// 如果查询到了指定的用户会返回这个用户结点，否则返回NULL
// 会将用户输入的用户名（数据文件中可以查询到的）赋值给target_username
UserNode* getSpecificUser(UserSystem*user_system,char target_username[]) {
    
        printf("\n可选用户列表:\n");
        printf("----------------------------------------\n");
        UserNode* user = user_system->head;
        while (user != NULL) {
            printf("%s\n", user->username);
            user = user->next;
        }
        printf("----------------------------------------\n");
        printf("请输入要进行操作的用户名: ");
        getValidatedStringInput(target_username, MAX_USERNAME_LENGTH);

        // 验证用户名是否存在
        int user_found = 0;
        user = user_system->head;
        while (user != NULL) {
            if (strcmp(user->username, target_username) == 0) {
                user_found = 1;
                return user;
                break;
            }
            user = user->next;
        }
        if (!user_found) {
            printf("----------------------------------------\n");
            printf("未找到该用户\n");
            printf("----------------------------------------\n");
            return NULL;
        }
    
    return NULL;
}

char* generateInviteCode() {
    char* invite_code = (char*)malloc(6 * sizeof(char));
    if (invite_code == NULL) {
        return NULL;
    }

    // 使用当前时间和进程ID作为种子以增加随机性
    srand((unsigned int)(time(NULL)));

    // 生成首位随机大写字母 (A-Z: 65-90)
    invite_code[0] = (char)(65 + rand() % 26);

    // 生成后四位随机数字
    for (int i = 1; i < 5; i++) {
        invite_code[i] = (char)(48 + rand() % 10);
    }

    // 添加字符串结束符
    invite_code[5] = '\0';
    return invite_code;
}