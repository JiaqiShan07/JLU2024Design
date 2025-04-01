#ifndef NOTIFICATION_HANDLER_H
#define NOTIFICATION_HANDLER_H
#include "all_h_files.h"
// 显示当前用户未读通知的数量，包括包裹状态变更等通知
void showNotificationCount(UserSystem* user_system,
                           PackageSystem* package_system);
//获取不同包裹的通知数量
int getDifferentTypePackageCount(UserSystem* user_system,
                           PackageSystem* package_system,PackageStatus type);
//展示不同包裹类型的细节
void showDifferentTypePackageDetails(UserSystem* user_system,
                           PackageSystem* package_system,PackageStatus type);
#endif  // NOTIFICATION_HANDLER_H