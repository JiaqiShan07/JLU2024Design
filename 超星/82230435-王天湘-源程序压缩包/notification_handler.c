#include "all_h_files.h"
// չʾ�û���֪ͨ����������Ա�Ϳ��Ա���Կ������а�����֪ͨ����ͨ�û�ֻ�ܿ����Լ��İ�����֪ͨ
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // ���ҵ�ǰ�û�
    UserNode* current_user = user_system->head;
    while (current_user != NULL &&
           strcmp(current_user->username, user_system->current_username) != 0) {
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        printf("��ǰ�û�������\n");
        return;
    }

    // �������а���ͳ��֪ͨ����
    while (current != NULL) {
        // ������״̬�Ƿ���Ҫ֪ͨ
        bool need_notification =
            (current->status == PENDING_PICKUP ||
             current->status == PENDING_DELIVERY ||
             current->status == ABNORMAL || current->status == REJECTED ||
             current->status == STRANDED);

        if (need_notification) {
            // ����Ա�Ϳ��Ա���Կ�������֪ͨ
            if (current_user->type == USER_ADMIN ||
                current_user->type == USER_COURIER) {
                count++;
            }
            // ��ͨ�û�ֻ�ܿ����Լ��İ���֪ͨ
            else if (strcmp(current->username, current_user->username) == 0) {
                count++;
            }
        }
        current = current->next;
    }

    printf("\n���� %d ����֪ͨ��\n", count);
}

// չʾ�û���ָ�����͵İ�����֪ͨ����
int getDifferentTypePackageCount(UserSystem* user_system,
                                 PackageSystem* package_system,
                                 PackageStatus type) {
    if (user_system == NULL || package_system == NULL) {
        return -1;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // ���ҵ�ǰ�û�
    UserNode* current_user = user_system->head;
    while (current_user != NULL &&
           strcmp(current_user->username, user_system->current_username) != 0) {
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        return -1;
    }

    // �������а���ͳ��ָ�����͵�����
    while (current != NULL) {
        if (current->status == type) {
            // ����Ա�Ϳ��Ա���Կ������а���
            if (current_user->type == USER_ADMIN ||
                current_user->type == USER_COURIER) {
                count++;
            }
            // ��ͨ�û�ֻ�ܿ����Լ��İ���
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
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // ���ҵ�ǰ�û�
    UserNode* user = user_system->head;
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }

    if (user == NULL) {
        printf("��ǰ�û�������\n");
        return;
    }

    int count = 0;
    PackageNode* current = package_system->head;

    // �����û����ͺͰ���״̬��ʾ������Ϣ
    while (current != NULL) {
        bool should_display = false;

        // �ж��Ƿ�Ӧ����ʾ�ð���
        if (user->type == USER_ADMIN || user->type == USER_COURIER) {
            should_display = (current->status == type);
        } else {
            should_display = (current->status == type &&
                              strcmp(current->username, user->username) == 0);
        }

        if (should_display) {
            count++;
            printf("----------------------------------------\n");
            printf("�������: %d\n", current->package_id);
            printf("״̬: %s\n", packageSatatusToString(current->status));
            printf("�����û�: %s\n", current->username);
            printf("ȡ����: %s\n", current->pickup_code);
            printf("���λ��: %s ��%d��\n",
                   switchShelfNumToString(current->shelf_number),
                   current->layer_number);
            printf("----------------------------------------\n");
        }
        current = current->next;
    }

    // ���û���ҵ���Ӧ���͵İ�������ʾ��ʾ��Ϣ
    if (count == 0) {
        printf("û��%s״̬�İ���!\n", packageSatatusToString(type));
    }
}