#include "all_h_files.h"
// ��ʾ��¼�˵�
// ��ӡϵͳ��¼���棬�����û�ע�ᡢ��¼���˳�ѡ��
void displayLoginMenu() {
    printf("\n----------------------------------------");
    printf("\n��ӭʹ�ÿ�ݹ���ϵͳ\n");
    printf("1. �û�ע��\n");
    printf("2. �û���¼\n");
    printf("0. �˳�ϵͳ\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
}

void displayMainMenu(UserSystem* user_system) {
    // ��ȡ��ǰ�û�����
    UserType current_user_type;

    UserNode* current = user_system->head;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            current_user_type = current->type;
            break;
        }
        current = current->next;
    }

    // ��ʾ�˵�ѡ��
    printf("\n----------------------------------------");
    printf("\n��ݹ���ϵͳ\n");
    printf(" 1. ��Ӱ���\n");
    printf(" 2. ��ѯ�û�����\n");
    printf(" 3. ȡ������\n");
    printf(" 4. ���հ���\n");
    printf(" 5. ���ⷴ��\n");
    printf(" 6. ��ϵ���ܿͷ�\n");
    printf(" 7. �޸��˻�����\n");
    printf(" 8. ע����ǰ�û�\n");
    // ����Ա�Ϳ��Ա�ɼ��Ķ���ѡ��
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf(" 9. ��ʾ���а���\n");
        printf("10. ����ͳ��\n");
        printf("11. �����ĳ�\n");
        printf("12. ����쳣����\n");

        // ������Ա�ɼ���ѡ��
        if (current_user_type == USER_ADMIN) {
            printf("13. ���ϵͳ����\n");
            printf("14. ע����Ա�˻�\n");
            printf("15. �򿪻��ܹ���ϵͳ\n");
            printf("16. �����û�������Ϣ\n");
            printf("17. ע�������û�\n");
        }
    }
    printf("0. �˳���¼\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
}
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system) {
    printf("\n----------------------------------------");
    showNotificationCount(user_system, package_system);
    printf("\n----------------------------------------\n");
    printf("\t֪ͨ�˵�\n");
    printf("1. �鿴֪ͨ����\n");
    printf("2. �������˵�\n");
    printf("0. �˳���¼\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
}