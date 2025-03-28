#include "all_h_files.h"
int handleLoginMenuInput(UserSystem* user_system,
                         PackageSystem* package_system) {
    int choice = getValidatedIntegerInput(0, 2, 1);
    switch (choice) {
        case 1:
            handleUserRegister(user_system);
            pauseAndClearConsole(1);
            return 0;
        case 2:
            handleUserLogin(user_system);
            if (user_system->is_login == true) {
                do {
                    displayNotificationMenu(user_system, package_system);
                } while (
                    !handleNotificationMenuInput(user_system, package_system));

                if (user_system->is_login == true) {
                    handleUserInput(package_system, user_system);
                }
            }
            return 0;
        case 0:
            pauseAndClearConsole(1);
            return 1;
        default:
            printf("无效的选择，请重试\n");
            pauseAndClearConsole(1);
            return 0;
    }
}
void handleUserInput(PackageSystem* system, UserSystem* user_system) {
    int choice;
    UserType current_user_type;

    UserNode* current = user_system->head;
    // 找到正确的登录用户
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            current_user_type = current->type;
            break;
        }
        current = current->next;
    }

    do {
        if (user_system->is_login == false) {
            printf("用户未登录，即将返回登录菜单...\n");
            Sleep(3000);
            pauseAndClearConsole(0);
            return;
        }
        displayMainMenu(user_system);
        // 分不同类型用户限制不同的菜单输入范围保证安全
        if(current_user_type == USER_ADMIN){
            choice = getValidatedIntegerInput(
                0, 19, 1);  // 第三个参数为1代表可以例外的输入0
        }else if(current_user_type == USER_COURIER){
            choice = getValidatedIntegerInput(
                0, 13, 1);  // 第三个参数为1代表可以例外的输入0 
        }else{
            choice = getValidatedIntegerInput(
                0, 11, 1);  // 第三个参数为1代表可以例外的输入0
        }
        switch (choice) {
            case 1:
                pauseAndClearConsole(0);
                handleAddPackage(system, user_system);
                pauseAndClearConsole(1);

                break;
            case 2:
                pauseAndClearConsole(0);
                handleQueryPackages(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 3:
                pauseAndClearConsole(0);
                handlePickupPackage(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 4:
                pauseAndClearConsole(0);
                handleRejectPackage(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 5:
                pauseAndClearConsole(0);
                handleFeedback(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 6:
                pauseAndClearConsole(0);
                handleContactSmartAssistant(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 7:
                pauseAndClearConsole(0);
                handleChangePassword(user_system);
                pauseAndClearConsole(1);
                break;
            case 8:
                pauseAndClearConsole(0);
                if (handleDeleteUserAccount(user_system,system)) {
                    printf("账户已注销，即将返回登录菜单...\n");
                    Sleep(3000);
                    pauseAndClearConsole(0);
                    return;
                } else {
                    printf("账户注销失败\n");
                    pauseAndClearConsole(1);
                }
                break;
            case 9:
                pauseAndClearConsole(0);
                do {
                    displayFriendMenu(user_system);
                } while (!handleFriendMenuInput(user_system));
                pauseAndClearConsole(1);
                break;
            case 10:
                pauseAndClearConsole(0);
                handlePickupPackageByOther(system, user_system);
                pauseAndClearConsole(1);
                break;

            case 11:
                pauseAndClearConsole(0);
                if (current_user_type !=USER_ADMIN && current_user_type != USER_COURIER && current_user_type != USER_VIP) {
                    handleUpgradeVIP(user_system);
                }
                else if (current_user_type == USER_ADMIN ||
                    current_user_type == USER_COURIER) {
                    handleDeliverPackage(system);
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;

            case 12:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN ||
                    current_user_type == USER_COURIER) {
                    handleMarkAbnormalPackage(system, user_system);
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;
            case 13:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN ||
                    current_user_type == USER_COURIER) {
                    handleStrandedPackages(system, user_system);
                }
                pauseAndClearConsole(1);
                break;
            case 14:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                    if (handleClearSystemData(system, user_system)) {
                        printf("系统数据已清空，即将返回登录菜单...\n");
                        Sleep(3000);
                        pauseAndClearConsole(0);
                        return;
                    }
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;

            case 15:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                    handleCourierRegister(user_system);
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;

            case 16:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                    ShelfSystem* shelf_system = initShelfSystem();
                    updateShelfStatus(shelf_system, system);
                    checkShelfCapacityWarning(shelf_system);
                    int shelf_choice;
                    do {
                        displayWarningMessage(shelf_system);
                        displayShelfMenu(user_system, system);
                    } while (!handleShelfMenuInput(system, user_system,
                                                   shelf_system));
                    clearShelfSystem(shelf_system);
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;

            case 17:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                    displayFeedbackList(system);
                } else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;
            case 18:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                   handleCleanInvalidPackageNode(system); 
                }
                pauseAndClearConsole(1);
                break;
            case 19:
                pauseAndClearConsole(0);
                if (current_user_type == USER_ADMIN) {
                    do{
                        displayAdminLogMenu();
                    } while (handleadmindiary(user_system, system));
                }
                else {
                    printf("无效的选择，请重试\n");
                }
                pauseAndClearConsole(1);
                break;
            case 0:
                pauseAndClearConsole(0);
                logoutUser(user_system);
                pauseAndClearConsole(1);
                return;
                break;

            default:
                printf("无效的选择，请重试\n");
                pauseAndClearConsole(1);
                break;
        }
    } while (choice != 0);
}
int handleNotificationMenuInput(UserSystem* user_system,
                                PackageSystem* package_system) {
    int choice = getValidatedIntegerInput(0, 2, 1);

    switch (choice) {
        case 1:
            pauseAndClearConsole(0);
            showNotificationDetails(user_system, package_system);
            pauseAndClearConsole(1);
            return 0;

        case 2:
            pauseAndClearConsole(0);
            return 1;

        case 0:
            logoutUser(user_system);
            pauseAndClearConsole(1);
            return 1;

        default:
            printf("无效的选择，请重试\n");
            pauseAndClearConsole(1);
            return 0;
    }
}
int handleShelfMenuInput(PackageSystem* system,
                         UserSystem* user_system,
                         ShelfSystem* shelf_system) {
    int choice = getValidatedIntegerInput(0, 3, 1);
    switch (choice) {
        case 1:
            pauseAndClearConsole(0);
            handlePackageStatistics(system, user_system);
            pauseAndClearConsole(1);
            return 0;
        case 2:
            pauseAndClearConsole(0);
            displayShelfUsage(shelf_system, system);
            pauseAndClearConsole(1);
            return 0;
        case 3:
            pauseAndClearConsole(0);
            displayAllPackages(system);
            pauseAndClearConsole(1);
            return 0;
        case 0:
            return 1;
        default:
            printf("无效的选择，请重试\n");
            pauseAndClearConsole(1);
            return 0;
    }
}

int handleFriendMenuInput(UserSystem* user_system) {
    int choice = getValidatedIntegerInput(0, 4, 1);

    switch (choice) {
        case 1:
            pauseAndClearConsole(0);
            handleAddFriends(user_system);
            pauseAndClearConsole(1);
            return 0;

        case 2:
            pauseAndClearConsole(0);
            handleViewFriends(user_system);
            pauseAndClearConsole(1);
            return 0;

        case 3:
            pauseAndClearConsole(0);
            handleRemoveFriends(user_system);
            pauseAndClearConsole(1);
            return 0;

        case 4:
            pauseAndClearConsole(0);
            handleViewInviteCode(user_system);
            pauseAndClearConsole(1);
            return 0;

        case 0:
            return 1;

        default:
            printf("无效的选择，请重试\n");
            pauseAndClearConsole(1);
            return 0;
    }
}
