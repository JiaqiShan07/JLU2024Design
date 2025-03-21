#ifndef UI_HANDLER_H
#define UI_HANDLER_H
#include "all_h_files.h"
#include "package_system.h"
#include "user_system.h"
void displayLoginMenu();
void displayMainMenu(UserSystem* user_system);
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system);
void displayFriendMenu(UserSystem* user_system);
void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system);

#endif