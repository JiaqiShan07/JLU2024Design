#include "all_h_files.h"
/**
 * ��ʾ��ǰ�û���֪ͨ����
 * �����û�������ʾ��ͬ��Χ��֪ͨ��
 * - ����Ա�Ϳ��Ա����ʾϵͳ�����д�ȡ������Ͷ�ݡ��쳣�����պ������İ�������
 * - ��ͨ�û�������ʾ���Լ���صĴ�ȡ������Ͷ�ݡ��쳣�����պ������İ�������
 */
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
    printf("\n���� %d ����֪ͨ��\n", count);
}

/**
 * ��ʾ��ǰ�û���֪ͨ��ϸ��Ϣ
 *
 * �����û�������ʾ��ͬ��Χ��֪ͨ���飺
 * -
 * ����Ա�Ϳ��Ա����ʾϵͳ�����д�ȡ������Ͷ�ݡ��쳣�����պ�������������ϸ��Ϣ
 *   ������������š�״̬�������û���ȡ����ʹ洢λ��
 * - ��ͨ�û�������ʾ���Լ���صĴ�ȡ������Ͷ�ݡ��쳣�����պ�������������ϸ��Ϣ
 *   ������������š�״̬��ȡ����ʹ洢λ��
 */
void showNotificationDetails(UserSystem* user_system,
                             PackageSystem* package_system) {
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
                    printf("�������: %d\n", current->package_id);
                    printf("״̬: %s\n",
                           packageSatatusToString(current->status));
                    if (user_type == USER_ADMIN || user_type == USER_COURIER) {
                        printf("�����û�: %s\n", current->username);
                    }
                    printf("ȡ����: %s\n", current->pickup_code);
                    printf("���λ��: %d�Ź� ��%d��\n", current->shelf_number,
                           current->layer_number);
                    printf("----------------------------------------\n");
                }
            }
            current = current->next;
        }

        if (!found && page == 1) {
            printf("û����Ҫ��ʾ��֪ͨ\n");
            printf("----------------------------------------\n");
            break;
        }

        // ����Ƿ��и���֪ͨ
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
            printf("�Ƿ�鿴����֪ͨ��(Y/N): ");
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