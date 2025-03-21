#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// 显示当前用户未读通知的数量，包括包裹状态变更等通知
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
// 显示当前用户的所有通知详细信息，包括包裹状态变更、系统提醒等
void showNotificationDetails(UserSystem* user_system,
                             PackageSystem* package_system);
#endif  // NOTIFICATION_HANDLER_H