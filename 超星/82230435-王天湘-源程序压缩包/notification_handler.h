#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// ��ʾ��ǰ�û�δ��֪ͨ����������������״̬�����֪ͨ
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
//��ȡ��ͬ������֪ͨ����
int getDifferentTypePackageCount(UserSystem* user_system,
                           PackageSystem* package_system,PackageStatus type);
//չʾ��ͬ�������͵�ϸ��
void showDifferentTypePackageDetails(UserSystem* user_system,
                           PackageSystem* package_system,PackageStatus type);
#endif  // NOTIFICATION_HANDLER_H