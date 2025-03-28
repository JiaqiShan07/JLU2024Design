#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// 显示当前用户未读通知的数量，包括包裹状态变更等通知
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
// 展示各类包裹的通知详情
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