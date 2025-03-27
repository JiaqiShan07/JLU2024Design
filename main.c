#include "all_h_files.h"
int main() {
    // ��ʼ���û�ϵͳ�Ͱ���ϵͳ
    UserSystem* user_system = initUserSystem();
    PackageSystem* package_system = initPackageSystem();
    if (user_system == NULL || package_system == NULL) {
        printf("ϵͳ��ʼ��ʧ��\n");
        return 1;
    }
    // ������ѭ��
    // should_exitΪ1ʱ�˳�ϵͳ
    int should_exit = 0;
    while (!should_exit) {
        // ��ʾ��¼�˵��������û�����,����ȷ��ÿ���˻ص���¼�˵����¼�������
        if (!loadUsersFromFile(user_system, USER_FILE)) {
            printf("�޷������û����ݣ��������µ������ļ�\n");
        }
        if (!loadPackagesFromFile(package_system, PACKAGE_FILE)) {
            printf("�޷����ذ������ݣ��������µ������ļ�\n");
        }
        displayLoginMenu();
        should_exit = handleLoginMenuInput(user_system, package_system);
    }
    // �˳�ǰ�ٴα����û����ݵ��ļ�
    if (!saveUsersToFile(user_system, USER_FILE)) {
        printf("�����û�����ʧ��\n");
    }
    // �˳�ǰ�ٴα���������ݵ��ļ�
    if (!savePackagesToFile(package_system, PACKAGE_FILE)) {
        printf("�����������ʧ��\n");
    }
    // �ͷ�ϵͳ��Դ
   //freeUserSystem(user_system);
    //freePackageSystem(package_system);
    PackageNode* curr = package_system->head;
    while (curr != NULL) {
        PackageNode* next = curr->next;
        free(curr);
        curr = curr->next;
    }
    UserNode*curr1 = user_system->head;
    while (curr1!= NULL) {
        UserNode* next = curr1->next;
        free(curr1);
        curr1 = curr1->next;
    }
    printf("\t-----------------\n");
    printf("\t��лʹ�ã��ټ���\n");
    printf("\t-----------------\n");
    Sleep(3000);
    return 0;
}