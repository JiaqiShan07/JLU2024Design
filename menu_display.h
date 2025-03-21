#ifndef UI_HANDLER_H
#define UI_HANDLER_H
#include "all_h_files.h"
#include "package_system.h"
#include "user_system.h"
// ��ʾ��¼�˵����棬������¼��ע��ѡ��
void displayLoginMenu();
// ��ʾ���˵����棬�����û�������ʾ��ͬ�Ĺ���ѡ��
void displayMainMenu(UserSystem* user_system);
// ��ʾ֪ͨ�˵����棬չʾ�û��İ������֪ͨ
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system);
// ��ʾ���ѹ���˵����棬������ӡ�ɾ���Ͳ鿴���ѵȹ���
void displayFriendMenu(UserSystem* user_system);
// ��ʾ���ܹ���˵����棬չʾ����ʹ��״̬�Ͱ����洢���
void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system);

#endif