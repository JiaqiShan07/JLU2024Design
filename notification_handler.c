#include "all_h_files.h"
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("系统未初始化\n");
        return;
    }
    int count = 0;
    PackageNode* current = package_system->head;
    UserType user_type = USER_NORMAL;
    UserNode* user = user_system->head;
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            user_type = user->type;
            break;
        }
        user = user->next;
    }
    while (current != NULL) {
        if (user_type == USER_ADMIN || user_type == USER_COURIER) {
            if (current->status == PENDING_PICKUP ||
                current->status == PENDING_DELIVERY ||
                current->status == ABNORMAL || current->status == REJECTED) {
                count++;
            }
        } else {
            if (strcmp(current->username, user->username) == 0) {
                if (current->status == PENDING_PICKUP ||
                    current->status == PENDING_DELIVERY ||
                    current->status == ABNORMAL ||
                    current->status == REJECTED) {
                    count++;
                }
            }
        }
        current = current->next;
    }
    printf("\n您有 %d 条新通知！\n", count);
}
void showNotificationDetails(UserSystem* user_system,PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("系统未初始化\n");
        return;
    }
    UserType user_type = USER_NORMAL;
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            user_type = user->type;
            strncpy(username, user->username, MAX_USERNAME_LENGTH);
            break;
        }
        user = user->next;
    }

    printf("\n通知详情：\n");
    printf("----------------------------------------\n");

    PackageNode* current = package_system->head;
    int found = 0;
    while (current != NULL) {
        int should_display = 0;
        if (user_type == USER_ADMIN || user_type == USER_COURIER) {
            should_display =
                (current->status == PENDING_PICKUP ||
                 current->status == PENDING_DELIVERY ||
                 current->status == ABNORMAL || current->status == REJECTED);
        } else {
            should_display =
                (strcmp(current->username, username) == 0 &&
                 (current->status == PENDING_PICKUP ||
                  current->status == PENDING_DELIVERY ||
                  current->status == ABNORMAL || current->status == REJECTED));
        }
        if (should_display) {
            found = 1;
            printf("包裹编号: %d\n", current->package_id);
            printf("状态: %s\n", packageSatatusToString(current->status));
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                printf("所属用户: %s\n", current->username);
            }
            printf("取件码: %s\n", current->pickup_code);
            printf("存放位置: %d号柜 第%d层\n", current->shelf_number,current->layer_number);
            printf("----------------------------------------\n");
        }
        current = current->next;
    }
    if (!found) {
        printf("没有需要显示的通知\n");
        printf("----------------------------------------\n");
    }
}