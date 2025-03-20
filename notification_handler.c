#include "all_h_files.h"
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
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
    printf("\n���� %d ����֪ͨ��\n", count);
}
void showNotificationDetails(UserSystem* user_system,PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
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

    printf("\n֪ͨ���飺\n");
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
            printf("�������: %d\n", current->package_id);
            printf("״̬: %s\n", packageSatatusToString(current->status));
            if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                printf("�����û�: %s\n", current->username);
            }
            printf("ȡ����: %s\n", current->pickup_code);
            printf("���λ��: %d�Ź� ��%d��\n", current->shelf_number,current->layer_number);
            printf("----------------------------------------\n");
        }
        current = current->next;
    }
    if (!found) {
        printf("û����Ҫ��ʾ��֪ͨ\n");
        printf("----------------------------------------\n");
    }
}