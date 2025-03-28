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
void showNotificationPENDING_PICKUPDetails(UserSystem* user_system,
                             PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    strncpy(username, user->username, MAX_USERNAME_LENGTH);
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }
    if (user->type == USER_ADMIN || user->type == USER_COURIER) {
        PackageNode* current = package_system->head;
        while (current!=NULL) {
            if (current->status == PENDING_PICKUP) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
    else {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (strcmp(current->username, user->username) == 0 && current->status == PENDING_PICKUP) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
}
void showNotificationPENDING_DELIVERYDetails(UserSystem* user_system,
    PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    strncpy(username, user->username, MAX_USERNAME_LENGTH);
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }
    if (user->type == USER_ADMIN || user->type == USER_COURIER) {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (current->status == PENDING_DELIVERY) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
    else {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (strcmp(current->username, user->username) == 0
                && current->status == PENDING_DELIVERY) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
}
void showNotificationABNORMALDetails(UserSystem* user_system,
    PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    strncpy(username, user->username, MAX_USERNAME_LENGTH);
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }
    if (user->type == USER_ADMIN || user->type == USER_COURIER) {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (current->status == ABNORMAL) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
    else {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (strcmp(current->username, user->username) == 0
                && current->status == ABNORMAL) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
}
void showNotificationSTRANDEDDetails(UserSystem* user_system,
    PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    strncpy(username, user->username, MAX_USERNAME_LENGTH);
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }
    if (user->type == USER_ADMIN || user->type == USER_COURIER) {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (current->status == STRANDED) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
    else {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (strcmp(current->username, user->username) == 0
                && current->status == STRANDED) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
}
void showNotificationREJECTEDDetails(UserSystem* user_system,
    PackageSystem* package_system) {
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }
    char username[MAX_USERNAME_LENGTH];
    UserNode* user = user_system->head;
    strncpy(username, user->username, MAX_USERNAME_LENGTH);
    while (user != NULL) {
        if (strcmp(user->username, user_system->current_username) == 0) {
            break;
        }
        user = user->next;
    }
    if (user->type == USER_ADMIN || user->type == USER_COURIER) {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (current->status == REJECTED) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
    else {
        PackageNode* current = package_system->head;
        while (current != NULL) {
            if (strcmp(current->username, user->username) == 0
                && current->status == REJECTED) {
                printf("----------------------------------------\n");
                printf("�������: %d\n", current->package_id);
                printf("״̬: %s\n", packageSatatusToString(current->status));
                printf("�����û�: %s\n", current->username);
                printf("ȡ����: %s\n", current->pickup_code);
                printf("���λ��: %s ��%d��\n", switchShelfNumToString(current->shelf_number),
                    current->layer_number);
                printf("----------------------------------------\n");
            }
            current = current->next;
        }
    }
}