#ifndef UI_HANDLER_H
#define UI_HANDLER_H
#include "all_h_files.h"
void displayLoginMenu();
void displayMainMenu(UserSystem* user_system);
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system);
#endif