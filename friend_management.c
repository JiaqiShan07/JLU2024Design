#include "all_h_files.h"
// 添加好友，添加好友是双向的，添加的时候需要输入对方的用户名,以及对方的邀请码
// 添加后对方的列表也会出现自己
// 参数：user_system - 用户管理系统指针
void handleAddFriends(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    char friend_username[MAX_USERNAME_LENGTH];
    char invite_code[10];
    printf("请输入要添加的好友用户名: ");
    if (!getSpecificUser(user_system, friend_username)) {
        return;
    }
    // 检查是否是自己
    if (strcmp(friend_username, user_system->current_username) == 0) {
        printf("不能添加自己为好友\n");
        return;
    }

    // 查找当前用户节点
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            // 检查好友数量是否达到上限
            if (current_user->friend_count >= MAX_FRIENDS) {
                printf("好友数量已达到上限\n");
                return;
            }

            // 检查是否已经是好友
            for (int i = 0; i < current_user->friend_count; i++) {
                if (strcmp(current_user->friends[i], friend_username) == 0) {
                    printf("该用户已经是您的好友\n");
                    return;
                }
            }
            break;
        }
        current_user = current_user->next;
    }

    printf("请输入好友的邀请码: ");
    getValidatedNumAndLetterInput(invite_code, 0, 10);

    // 调用核心逻辑函数添加好友
    addFriends(user_system, friend_username, invite_code);
}

void handleViewFriends(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    // 查找当前用户节点
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            printf("\n----------------------------------------\n");
            printf("好友列表 (共%d个好友)\n", current_user->friend_count);
            printf("----------------------------------------\n");

            if (current_user->friend_count == 0) {
                printf("暂无好友\n");
            } else {
                for (int i = 0; i < current_user->friend_count; i++) {
                    printf("%d. %s\n", i + 1, current_user->friends[i]);
                }
            }
            printf("----------------------------------------\n");
            return;
        }
        current_user = current_user->next;
    }
    printf("获取用户信息失败\n");
}

void handleViewInviteCode(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    // 查找当前用户节点
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            printf("\n----------------------------------------\n");
            printf("我的邀请码\n");
            printf("----------------------------------------\n");
            printf("邀请码: %s\n", current_user->invateCode);
            printf("----------------------------------------\n");
            return;
        }
        current_user = current_user->next;
    }
    printf("获取用户信息失败\n");
}

void addFriends(UserSystem* user_system,
                const char* friend_username,
                const char* invite_code) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    // 查找目标用户
    UserNode* target_user = user_system->head;
    UserNode* current_user = NULL;

    // 找到当前用户和目标用户的节点
    while (target_user != NULL) {
        if (strcmp(target_user->username, friend_username) == 0) {
            // 验证邀请码
            if (strcmp(target_user->invateCode, invite_code) != 0) {
                printf("邀请码错误\n");
                return;
            }
            // 检查目标用户的好友列表是否已满
            if (target_user->friend_count >= MAX_FRIENDS) {
                printf("对方的好友列表已满\n");
                return;
            }
            break;
        }
        target_user = target_user->next;
    }

    if (target_user == NULL) {
        printf("未找到用户 %s\n", friend_username);
        return;
    }

    // 找到当前用户节点
    current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            break;
        }
        current_user = current_user->next;
    }

    // 双向添加好友
    // 将目标用户添加到当前用户的好友列表
    strncpy(current_user->friends[current_user->friend_count], friend_username,
            MAX_USERNAME_LENGTH - 1);
    current_user->friends[current_user->friend_count][MAX_USERNAME_LENGTH - 1] =
        '\0';
    current_user->friend_count++;

    // 将当前用户添加到目标用户的好友列表
    strncpy(target_user->friends[target_user->friend_count],
            user_system->current_username, MAX_USERNAME_LENGTH - 1);
    target_user->friends[target_user->friend_count][MAX_USERNAME_LENGTH - 1] =
        '\0';
    target_user->friend_count++;

    // 保存更新到文件
    if (saveUsersToFile(user_system, USER_FILE)) {
        printf("已成功添加好友 %s\n", friend_username);
    } else {
        printf("添加好友成功，但保存到文件失败\n");
    }
}

void deleteFriend(UserSystem* user_system, const char* friend_username) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    // 查找当前用户节点
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            break;
        }
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        printf("获取用户信息失败\n");
        return;
    }

    // 在当前用户的好友列表中查找并删除目标好友
    int found = 0;
    for (int i = 0; i < current_user->friend_count; i++) {
        if (strcmp(current_user->friends[i], friend_username) == 0) {
            // 将后面的好友向前移动
            for (int j = i; j < current_user->friend_count - 1; j++) {
                strncpy(current_user->friends[j], current_user->friends[j + 1],
                        MAX_USERNAME_LENGTH - 1);
                current_user->friends[j][MAX_USERNAME_LENGTH - 1] = '\0';
            }
            current_user->friend_count--;
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("%s 不是您的好友\n", friend_username);
        return;
    }

    // 查找目标好友节点
    UserNode* friend_node = user_system->head;
    while (friend_node != NULL) {
        if (strcmp(friend_node->username, friend_username) == 0) {
            break;
        }
        friend_node = friend_node->next;
    }

    if (friend_node == NULL) {
        printf("目标用户不存在\n");
        return;
    }

    // 在目标好友的好友列表中删除当前用户
    for (int i = 0; i < friend_node->friend_count; i++) {
        if (strcmp(friend_node->friends[i], user_system->current_username) ==
            0) {
            // 将后面的好友向前移动
            for (int j = i; j < friend_node->friend_count - 1; j++) {
                strncpy(friend_node->friends[j], friend_node->friends[j + 1],
                        MAX_USERNAME_LENGTH - 1);
                friend_node->friends[j][MAX_USERNAME_LENGTH - 1] = '\0';
            }
            friend_node->friend_count--;
            break;
        }
    }
}

void handleRemoveFriends(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("请先登录\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("删除好友\n");
    printf("----------------------------------------\n");

    // 显示当前好友列表
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            if (current_user->friend_count == 0) {
                printf("您暂无好友\n");
                printf("----------------------------------------\n");
                return;
            }

            printf("当前好友列表：\n");
            for (int i = 0; i < current_user->friend_count; i++) {
                printf("%d. %s\n", i + 1, current_user->friends[i]);
            }
            break;
        }
        current_user = current_user->next;
    }

    char friend_username[MAX_USERNAME_LENGTH];
    printf("\n请输入要删除的好友用户名: ");
    getValidatedNumAndLetterInput(friend_username, MIN_USERNAME_PASSWORD_LENGTH,
                                  MAX_USERNAME_LENGTH);

    // 确认删除
    printf("确认要删除好友 %s 吗？(y/n): ", friend_username);
    char confirm = getValidatedCharInput("YNyn");

    if (confirm == 'y' || confirm == 'Y') {
        deleteFriend(user_system, friend_username);
        // 保存更新到文件
        if (saveUsersToFile(user_system, USER_FILE)) {
            printf("已成功删除好友 %s\n", friend_username);
        } else {
            printf("删除好友成功，但保存到文件失败\n");
        }
    } else {
        printf("操作已取消\n");
    }
    printf("----------------------------------------\n");
}