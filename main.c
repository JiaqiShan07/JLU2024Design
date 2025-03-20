#include "all_h_files.h"
int main() {
    // 初始化用户系统和包裹系统
    UserSystem* user_system = initUserSystem();
    PackageSystem* package_system = initPackageSystem();
    if (user_system == NULL || package_system == NULL) {
        printf("系统初始化失败\n");
        return 1;
    }
    // 从文件加载历史数据
    // 如果数据文件不存在，将创建新的数据文件
    if (!loadUsersFromFile(user_system, USER_FILE)) {
        printf("无法加载用户数据，将创建新的数据文件\n");
    }
    if (!loadPackagesFromFile(package_system, PACKAGE_FILE)) {
        printf("无法加载包裹数据，将创建新的数据文件\n");
    }
    // 主程序循环
    // should_exit为1时退出系统
    int should_exit = 0;
    while (!should_exit) {
        // 显示登录菜单并处理用户输入
        displayLoginMenu();
        should_exit = handleLoginMenuInput(user_system, package_system);
    }
    // 退出前再次保存用户数据到文件
    if (!saveUsersToFile(user_system, USER_FILE)) {
        printf("保存用户数据失败\n");
    }
    // 退出前再次保存包裹数据到文件
    if (!savePackagesToFile(package_system, PACKAGE_FILE)) {
        printf("保存包裹数据失败\n");
    }
    // 释放系统资源
    free(user_system);
    free(package_system);
    printf("-----------------\n");
    printf("感谢使用，再见！\n");
    printf("-----------------\n");
    Sleep(3000);
    return 0;
}