#ifndef INPUT_MANAGEMENT_H
#define INPUT_MANAGEMENT_H
#include "package_system.h"
#include "user_system.h"
#include "feedback_assistant.h"
int handleLoginMenuInput(UserSystem* user_system, PackageSystem* package_system);
void handleUserInput(PackageSystem* system, UserSystem* user_system);
int handleNotificationMenuInput(UserSystem* user_system, PackageSystem* package_system);
#endif // INPUT_MANAGEMENT_H