#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// ��ʾ��ǰ�û�δ��֪ͨ����������������״̬�����֪ͨ
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
// ��ʾ��ǰ�û�������֪ͨ��ϸ��Ϣ����������״̬�����ϵͳ���ѵ�
void showNotificationDetails(UserSystem* user_system,
                             PackageSystem* package_system);
#endif  // NOTIFICATION_HANDLER_H