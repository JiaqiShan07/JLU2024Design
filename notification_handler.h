#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// ��ʾ��ǰ�û�δ��֪ͨ����������������״̬�����֪ͨ
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
// չʾ���������֪ͨ����
void showNotificationPENDING_PICKUPDetails(UserSystem* user_system,
                                           PackageSystem* package_system);
void showNotificationPENDING_DELIVERYDetails(UserSystem* user_system,
                                             PackageSystem* package_system);
void showNotificationABNORMALDetails(UserSystem* user_system,
                                     PackageSystem* package_system);
void showNotificationSTRANDEDDetails(UserSystem* user_system,
                                     PackageSystem* package_system);
void showNotificationREJECTEDDetails(UserSystem* user_system,
                                     PackageSystem* package_system);
#endif  // NOTIFICATION_HANDLER_H