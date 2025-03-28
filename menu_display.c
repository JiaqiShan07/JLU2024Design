#include "all_h_files.h"
// ��ʾ��¼�˵�
// ��ӡϵͳ��¼���棬�����û�ע�ᡢ��¼���˳�ѡ��
void displayLoginMenu() {
    printf("----------��ӭʹ�ÿ�ݹ���ϵͳ--------------\n");
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
    printf("-----------------���˵�--------------------\n");
    printf("---------------�ռļ�ϵͳ------------------\n");
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf(" 1. ������\n");
    } else {
        printf(" 1. �ʼİ���\n");
    }

    printf(" 2. ��ѯ�û�����\n");
    printf(" 3. ȡ������\n");
    printf(" 4. ���հ���\n");
    printf("--------------���ѹ���ϵͳ----------------\n");
    printf(" 5. ����ϵͳ\n");
    printf(" 6. ��ȡ���Ѱ���\n");
    printf("------------------����-------------------\n");
    printf(" 7. ���ⷴ��\n");
    printf(" 8. ��ϵ��������\n");
    printf("---------------���˹���ϵͳ--------------\n");
    printf(" 9. �޸��˻�����\n");
    printf("10. �˻�ע��\n");
    // ��VIP�û��ɼ�������ѡ��
    if ((current_user_type != USER_ADMIN) &&
        (current_user_type != USER_COURIER) &&
        (current_user_type != USER_VIP)) {
        printf("11. ������ΪVIP\n");
        printf("----------------------------------------\n");
    }
    if (current_user_type == USER_VIP) {
        printf("----------------------------------------\n");
    }
    // ����Ա�Ϳ��Ա�ɼ��Ķ���ѡ��
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf("----------�ĳ��������쳣����ϵͳ----------\n");
        printf("11. �����ĳ�\n");
        printf("12. ����쳣����\n");
        printf("13. ������������\n");
        // ������Ա�ɼ���ѡ��
        if (current_user_type == USER_ADMIN) {
            printf("14. �������ð�����¼\n");
            printf("------------����Աϵͳ����---------------\n");
            printf("15. ע����Ա�˻�\n");
            printf("16. �򿪻��ܹ���ϵͳ\n");
            printf("17. �����û�������Ϣ\n");
            printf("18. ���ϵͳ����\n");
            printf("19. �鿴����Ա��־\n");
            printf("----------------------------------------\n");
        }
    }
    printf(" 0. �˳���¼\n");
    printf("��ѡ�����: ");
}
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system) {
    printf("\n----------------------------------------");
    showNotificationCount(user_system, package_system);
    printf("\n--------------֪ͨ�˵�-------------------\n");
    printf("1. �鿴֪ͨ����\n");
    printf("2. �������˵�\n");
    printf("0. �˳���¼\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
}
void displaysecondNotificationMenu(UserSystem* user_system,
                                   PackageSystem* package_system) {
    printf("\n-------------����֪ͨ����--------------\n");
    printf("1. ȡ��֪ͨ\n");
    printf("2. ���ĳ�֪ͨ\n");
    printf("3. �쳣֪ͨ\n");
    printf("4. ����֪ͨ\n");
    printf("5. ����֪ͨ\n");
    printf("0. �˳�֪ͨ����\n");
    printf("----------------------------------------\n");
    printf("��ѡ�����: ");
}

void displayFriendMenu(UserSystem* user_system) {
    printf("------------------����ϵͳ-----------------\n");
    printf("1. ��Ӻ���\n");
    printf("2. �鿴�����б�\n");
    printf("3. ɾ������\n");
    printf("4. �鿴�ҵ�������\n");
    printf("0. �������˵�\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
}

void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system) {
    printf("----------------���ܹ���ϵͳ---------------\n");
    printf("\n----------------------------------------\n");
    printf("1. �ſ�ͳ��\n");
    printf("2. �ֻ���ͳ��\n");
    printf("3. ��ʾȫ������\n");
    printf("0. �������˵�\n");
    printf("��ѡ�����: ");
    printf("\n----------------------------------------\n");
    printf("���ܵķ���ԭ��\n");
    printf("���ݰ������ͺʹ�С������ܺţ������������\n");
    printf("1-3�Ż��ֱܷ���ô���С�ŷ���ذ���\n");
    printf("4�Ż���ר�ŷ�����ذ���\n");
    printf("5�Ż���ר�ŷ��ù��ذ���");
    printf("\n----------------------------------------\n");
}
// չʾ����Ա��־��ϵͳ�Ĳ˵�
void displayAdminLogMenu() {
    printf("-------------��ˮ��־�˵�-----------------\n");
    printf("��ѡ����Ҫ�鿴����ˮ��Ŀ:\n");
    printf("1.�û���ֵVIP��ˮ\n");
    printf("2.�û��İ�����ˮ\n");
    printf("3.����Ա�����ˮ��¼\n");
    printf("0.����\n");
    printf("----------------------------------------\n");
    printf("��ѡ�� (0-3): ");
}
void displayFeedbackMenu() {
    printf("\n----------------------------------------\n");
    printf("����ϵͳ\n");
    printf("1. �ύ�·���\n");
    printf("2. �鿴�ҵķ���\n");
    printf("0. �������˵�\n");
    printf("----------------------------------------\n");
    printf("��ѡ�����: ");
}