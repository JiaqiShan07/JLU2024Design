#include "all_h_files.h"
// 展示用户的通知数量，管理员和快递员可以看到所有包裹的通知，普通用户只能看到自己的包裹的通知
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // 查找当前用户
    UserNode* current_user = user_system->head;
    while (current_user != NULL &&
           strcmp(current_user->username, user_system->current_username) != 0) {
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        printf("当前用户不存在\n");
        return;
    }

    // 遍历所有包裹统计通知数量
    while (current != NULL) {
        // 检查包裹状态是否需要通知
        bool need_notification =
            (current->status == PENDING_PICKUP ||
             current->status == PENDING_DELIVERY ||
             current->status == ABNORMAL || current->status == REJECTED ||
             current->status == STRANDED);

        if (need_notification) {
            // 管理员和快递员可以看到所有通知
            if (current_user->type == USER_ADMIN ||
                current_user->type == USER_COURIER) {
                count++;
            }
            // 普通用户只能看到自己的包裹通知
            else if (strcmp(current->username, current_user->username) == 0) {
                count++;
            }
        }
        current = current->next;
    }

    printf("\n您有 %d 条新通知！\n", count);
}

// 展示用户的指定类型的包裹的通知数量
int getDifferentTypePackageCount(UserSystem* user_system,
                                 PackageSystem* package_system,
                                 PackageStatus type) {
    if (user_system == NULL || package_system == NULL) {
        return -1;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // 查找当前用户
    UserNode* current_user = user_system->head;
    while (current_user != NULL &&
           strcmp(current_user->username, user_system->current_username) != 0) {
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        return -1;
    }

    // 遍历所有包裹统计指定类型的数量
    while (current != NULL) {
        if (current->status == type) {
            // 管理员和快递员可以看到所有包裹
            if (current_user->type == USER_ADMIN ||
                current_user->type == USER_COURIER) {
                count++;
            }
            // 普通用户只能看到自己的包裹
            else if (strcmp(current->username, current_user->username) == 0) {
                count++;
            }
        }
        current = current->next;
    }

    return count;
}
void showDifferentTypePackageDetails(UserSystem* user_system,
                                     PackageSystem* package_system,
                                     PackageStatus type) {
    if (user_system == NULL || package_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    // 查找当前用户
    UserNode* user = user_system->head;
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }

    if (user == NULL) {
        printf("当前用户不存在\n");
        return;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // 根据用户类型和包裹状态显示包裹信息
    while (current != NULL) {
        bool should_display = false;

        // 判断是否应该显示该包裹
        if (user->type == USER_ADMIN || user->type == USER_COURIER) {
            should_display = (current->status == type);
        } else {
            should_display = (current->status == type &&
                              strcmp(current->username, user->username) == 0);
        }

        if (should_display) {
            count++;
            printf("----------------------------------------\n");
            printf("包裹编号: %d\n", current->package_id);
            printf("状态: %s\n", packageSatatusToString(current->status));
            printf("所属用户: %s\n", current->username);
            printf("取件码: %s\n", current->pickup_code);
            printf("存放位置: %s 第%d层\n",
                   switchShelfNumToString(current->shelf_number),
                   current->layer_number);
            printf("----------------------------------------\n");
        }
        current = current->next;
    }

    // 如果没有找到对应类型的包裹，显示提示信息
    if (count == 0) {
        printf("没有%s状态的包裹!\n", packageSatatusToString(type));
    }
}