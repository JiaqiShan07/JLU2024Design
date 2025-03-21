#ifndef UI_HANDLER_H
#define UI_HANDLER_H
#include "all_h_files.h"
#include "package_system.h"
#include "user_system.h"
// 显示登录菜单界面，包括登录和注册选项
void displayLoginMenu();
// 显示主菜单界面，根据用户类型显示不同的功能选项
void displayMainMenu(UserSystem* user_system);
// 显示通知菜单界面，展示用户的包裹相关通知
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system);
// 显示好友管理菜单界面，包括添加、删除和查看好友等功能
void displayFriendMenu(UserSystem* user_system);
// 显示货架管理菜单界面，展示货架使用状态和包裹存储情况
void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system);

#endif