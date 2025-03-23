#include "all_h_files.h"
// ��Ӻ��ѣ���Ӻ�����˫��ģ���ӵ�ʱ����Ҫ����Է����û���,�Լ��Է���������
// ��Ӻ�Է����б�Ҳ������Լ�
// ������user_system - �û�����ϵͳָ��
void handleAddFriends(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    char friend_username[MAX_USERNAME_LENGTH];
    char invite_code[10];
    printf("������Ҫ��ӵĺ����û���: ");
    if (!getSpecificUser(user_system, friend_username)) {
        return;
    }
    // ����Ƿ����Լ�
    if (strcmp(friend_username, user_system->current_username) == 0) {
        printf("��������Լ�Ϊ����\n");
        return;
    }

    // ���ҵ�ǰ�û��ڵ�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            // �����������Ƿ�ﵽ����
            if (current_user->friend_count >= MAX_FRIENDS) {
                printf("���������Ѵﵽ����\n");
                return;
            }

            // ����Ƿ��Ѿ��Ǻ���
            for (int i = 0; i < current_user->friend_count; i++) {
                if (strcmp(current_user->friends[i], friend_username) == 0) {
                    printf("���û��Ѿ������ĺ���\n");
                    return;
                }
            }
            break;
        }
        current_user = current_user->next;
    }

    printf("��������ѵ�������: ");
    getValidatedNumAndLetterInput(invite_code, 0, 10);

    // ���ú����߼�������Ӻ���
    addFriends(user_system, friend_username, invite_code);
}

void handleViewFriends(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    // ���ҵ�ǰ�û��ڵ�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            printf("\n----------------------------------------\n");
            printf("�����б� (��%d������)\n", current_user->friend_count);
            printf("----------------------------------------\n");

            if (current_user->friend_count == 0) {
                printf("���޺���\n");
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
    printf("��ȡ�û���Ϣʧ��\n");
}

void handleViewInviteCode(UserSystem* user_system) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    // ���ҵ�ǰ�û��ڵ�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            printf("\n----------------------------------------\n");
            printf("�ҵ�������\n");
            printf("----------------------------------------\n");
            printf("������: %s\n", current_user->invateCode);
            printf("----------------------------------------\n");
            return;
        }
        current_user = current_user->next;
    }
    printf("��ȡ�û���Ϣʧ��\n");
}

void addFriends(UserSystem* user_system,
                const char* friend_username,
                const char* invite_code) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    // ����Ŀ���û�
    UserNode* target_user = user_system->head;
    UserNode* current_user = NULL;

    // �ҵ���ǰ�û���Ŀ���û��Ľڵ�
    while (target_user != NULL) {
        if (strcmp(target_user->username, friend_username) == 0) {
            // ��֤������
            if (strcmp(target_user->invateCode, invite_code) != 0) {
                printf("���������\n");
                return;
            }
            // ���Ŀ���û��ĺ����б��Ƿ�����
            if (target_user->friend_count >= MAX_FRIENDS) {
                printf("�Է��ĺ����б�����\n");
                return;
            }
            break;
        }
        target_user = target_user->next;
    }

    if (target_user == NULL) {
        printf("δ�ҵ��û� %s\n", friend_username);
        return;
    }

    // �ҵ���ǰ�û��ڵ�
    current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            break;
        }
        current_user = current_user->next;
    }

    // ˫����Ӻ���
    // ��Ŀ���û���ӵ���ǰ�û��ĺ����б�
    strncpy(current_user->friends[current_user->friend_count], friend_username,
            MAX_USERNAME_LENGTH - 1);
    current_user->friends[current_user->friend_count][MAX_USERNAME_LENGTH - 1] =
        '\0';
    current_user->friend_count++;

    // ����ǰ�û���ӵ�Ŀ���û��ĺ����б�
    strncpy(target_user->friends[target_user->friend_count],
            user_system->current_username, MAX_USERNAME_LENGTH - 1);
    target_user->friends[target_user->friend_count][MAX_USERNAME_LENGTH - 1] =
        '\0';
    target_user->friend_count++;

    // ������µ��ļ�
    if (saveUsersToFile(user_system, USER_FILE)) {
        printf("�ѳɹ���Ӻ��� %s\n", friend_username);
    } else {
        printf("��Ӻ��ѳɹ��������浽�ļ�ʧ��\n");
    }
}

void deleteFriend(UserSystem* user_system, const char* friend_username) {
    if (user_system == NULL || user_system->is_login == false) {
        printf("���ȵ�¼\n");
        return;
    }

    // ���ҵ�ǰ�û��ڵ�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            break;
        }
        current_user = current_user->next;
    }

    if (current_user == NULL) {
        printf("��ȡ�û���Ϣʧ��\n");
        return;
    }

    // �ڵ�ǰ�û��ĺ����б��в��Ҳ�ɾ��Ŀ�����
    int found = 0;
    for (int i = 0; i < current_user->friend_count; i++) {
        if (strcmp(current_user->friends[i], friend_username) == 0) {
            // ������ĺ�����ǰ�ƶ�
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
        printf("%s �������ĺ���\n", friend_username);
        return;
    }

    // ����Ŀ����ѽڵ�
    UserNode* friend_node = user_system->head;
    while (friend_node != NULL) {
        if (strcmp(friend_node->username, friend_username) == 0) {
            break;
        }
        friend_node = friend_node->next;
    }

    if (friend_node == NULL) {
        printf("Ŀ���û�������\n");
        return;
    }

    // ��Ŀ����ѵĺ����б���ɾ����ǰ�û�
    for (int i = 0; i < friend_node->friend_count; i++) {
        if (strcmp(friend_node->friends[i], user_system->current_username) ==
            0) {
            // ������ĺ�����ǰ�ƶ�
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
        printf("���ȵ�¼\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("ɾ������\n");
    printf("----------------------------------------\n");

    // ��ʾ��ǰ�����б�
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (strcmp(current_user->username, user_system->current_username) ==
            0) {
            if (current_user->friend_count == 0) {
                printf("�����޺���\n");
                printf("----------------------------------------\n");
                return;
            }

            printf("��ǰ�����б�\n");
            for (int i = 0; i < current_user->friend_count; i++) {
                printf("%d. %s\n", i + 1, current_user->friends[i]);
            }
            break;
        }
        current_user = current_user->next;
    }

    char friend_username[MAX_USERNAME_LENGTH];
    printf("\n������Ҫɾ���ĺ����û���: ");
    getValidatedNumAndLetterInput(friend_username, MIN_USERNAME_PASSWORD_LENGTH,
                                  MAX_USERNAME_LENGTH);

    // ȷ��ɾ��
    printf("ȷ��Ҫɾ������ %s ��(y/n): ", friend_username);
    char confirm = getValidatedCharInput("YNyn");

    if (confirm == 'y' || confirm == 'Y') {
        deleteFriend(user_system, friend_username);
        // ������µ��ļ�
        if (saveUsersToFile(user_system, USER_FILE)) {
            printf("�ѳɹ�ɾ������ %s\n", friend_username);
        } else {
            printf("ɾ�����ѳɹ��������浽�ļ�ʧ��\n");
        }
    } else {
        printf("������ȡ��\n");
    }
    printf("----------------------------------------\n");
}