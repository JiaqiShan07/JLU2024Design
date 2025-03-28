#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
// 处理登录菜单的用户输入，包括登录和注册选项的选择
int handleLoginMenuInput(UserSystem* user_system,
                         PackageSystem* package_system);
// 处理主菜单的用户输入，根据用户类型提供不同的功能选项
void handleMainMenuInput(PackageSystem* system, UserSystem* user_system);
// 处理通知菜单的用户输入，包括查看和处理包裹状态通知
int handleNotificationMenuInput(UserSystem* user_system,
                                PackageSystem* package_system);
// 处理好友管理菜单的用户输入，包括添加、删除和查看好友
int handleFriendMenuInput(UserSystem* user_system);
// 处理货架管理菜单的用户输入，包括查看货架状态和包裹存储情况
int handleShelfMenuInput(PackageSystem* system,
                         UserSystem* user_system,
                         ShelfSystem* shelf_system);
// 处理管理员日志菜单的输入
int handleAdminLogMenuInput(UserSystem* user_system, PackageSystem* system);
int handlesecondNotificationMenuInput(UserSystem* user_system,
                                      PackageSystem* package_system);
#endif  // INPUT_MANAGEMENT_H