#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
// �����¼�˵����û����룬������¼��ע��ѡ���ѡ��
int handleLoginMenuInput(UserSystem* user_system,
                         PackageSystem* package_system);
// �������˵����û����룬�����û������ṩ��ͬ�Ĺ���ѡ��
void handleMainMenuInput(PackageSystem* system, UserSystem* user_system);
// ����֪ͨ�˵����û����룬�����鿴�ʹ������״̬֪ͨ
int handleNotificationMenuInput(UserSystem* user_system,
                                PackageSystem* package_system);
// ������ѹ���˵����û����룬������ӡ�ɾ���Ͳ鿴����
int handleFriendMenuInput(UserSystem* user_system);
// ������ܹ���˵����û����룬�����鿴����״̬�Ͱ����洢���
int handleShelfMenuInput(PackageSystem* system,
                         UserSystem* user_system,
                         ShelfSystem* shelf_system);
// �������Ա��־�˵�������
int handleAdminLogMenuInput(UserSystem* user_system, PackageSystem* system);
int handlesecondNotificationMenuInput(UserSystem* user_system,
                                      PackageSystem* package_system);
#endif  // INPUT_MANAGEMENT_H