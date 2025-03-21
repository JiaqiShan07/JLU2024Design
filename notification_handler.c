#include "all_h_files.h"
/**
 * 显示当前用户的通知数量
 * 根据用户类型显示不同范围的通知：
 * - 管理员和快递员：显示系统中所有待取件、待投递、异常、拒收和滞留的包裹数量
 * - 普通用户：仅显示与自己相关的待取件、待投递、异常、拒收和滞留的包裹数量
 */
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
                current->status == ABNORMAL || current->status == REJECTED ||
                current->status == STRANDED) {
                count++;
            }
        } else {
            if (strcmp(current->username, user->username) == 0) {
                if (current->status == PENDING_PICKUP ||
                    current->status == PENDING_DELIVERY ||
                    current->status == ABNORMAL ||
                    current->status == REJECTED ||
                    current->status == STRANDED) {
                    count++;
                }
            }
        }
        current = current->next;
    }
    printf("\n您有 %d 条新通知！\n", count);
}

/**
 * 显示当前用户的通知详细信息
 *
 * 根据用户类型显示不同范围的通知详情：
 * -
 * 管理员和快递员：显示系统中所有待取件、待投递、异常、拒收和滞留包裹的详细信息
 *   包括：包裹编号、状态、所属用户、取件码和存储位置
 * - 普通用户：仅显示与自己相关的待取件、待投递、异常、拒收和滞留包裹的详细信息
 *   包括：包裹编号、状态、取件码和存储位置
 */
void showNotificationDetails(UserSystem* user_system,
                             PackageSystem* package_system) {
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
    int displayed = 0;
    int page = 1;

    do {
        current = package_system->head;
        displayed = 0;
        int skip = (page - 1) * 5;
        int skipped = 0;

        while (current != NULL && displayed < 5) {
            int should_display = 0;
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                should_display = (current->status == PENDING_PICKUP ||
                                  current->status == PENDING_DELIVERY ||
                                  current->status == ABNORMAL ||
                                  current->status == REJECTED ||
                                  current->status == STRANDED);
            } else {
                should_display = (strcmp(current->username, username) == 0 &&
                                  (current->status == PENDING_PICKUP ||
                                   current->status == PENDING_DELIVERY ||
                                   current->status == ABNORMAL ||
                                   current->status == REJECTED ||
                                   current->status == STRANDED));
            }
            if (should_display) {
                if (skipped < skip) {
                    skipped++;
                } else {
                    found = 1;
                    displayed++;
                    printf("包裹编号: %d\n", current->package_id);
                    printf("状态: %s\n",
                           packageSatatusToString(current->status));
                    if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                        printf("所属用户: %s\n", current->username);
                    }
                    printf("取件码: %s\n", current->pickup_code);
                    printf("存放位置: %d号柜 第%d层\n", current->shelf_number,
                           current->layer_number);
                    printf("----------------------------------------\n");
                }
            }
            current = current->next;
        }

        if (!found && page == 1) {
            printf("没有需要显示的通知\n");
            printf("----------------------------------------\n");
            break;
        }

        // 检查是否还有更多通知
        int has_more = 0;
        while (current != NULL) {
            int should_display = 0;
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                should_display = (current->status == PENDING_PICKUP ||
                                  current->status == PENDING_DELIVERY ||
                                  current->status == ABNORMAL ||
                                  current->status == REJECTED ||
                                  current->status == STRANDED);
            } else {
                should_display = (strcmp(current->username, username) == 0 &&
                                  (current->status == PENDING_PICKUP ||
                                   current->status == PENDING_DELIVERY ||
                                   current->status == ABNORMAL ||
                                   current->status == REJECTED ||
                                   current->status == STRANDED));
            }
            if (should_display) {
                has_more = 1;
                break;
            }
            current = current->next;
        }

        if (has_more) {
            printf("是否查看更多通知？(Y/N): ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                page++;
                printf("\n");
            } else {
                break;
            }
        } else {
            break;
        }
    } while (1);
}