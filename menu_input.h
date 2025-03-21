#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H
#include "all_h_files.h"
#include "package_system.h"
#include "user_system.h"
#include "shelf_management.h"
int handleLoginMenuInput(UserSystem* user_system,
                         PackageSystem* package_system);
void handleUserInput(PackageSystem* system, UserSystem* user_system);
int handleNotificationMenuInput(UserSystem* user_system,
                                PackageSystem* package_system);
int handleFriendMenuInput(UserSystem* user_system);
int handleShelfMenuInput(PackageSystem* system,
                         UserSystem* user_system,
                         ShelfSystem* shelf_system);
#endif  // INPUT_MANAGEMENT_H