#include "all_h_files.h"
// 显示登录菜单
// 打印系统登录界面，包括用户注册、登录和退出选项
void displayLoginMenu() {
    printf("\n----------------------------------------");
    printf("\n欢迎使用快递管理系统\n");
    printf("1. 用户注册\n");
    printf("2. 用户登录\n");
    printf("0. 退出系统\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}

void displayMainMenu(UserSystem* user_system) {
    // 获取当前用户类型
    UserType current_user_type;

    UserNode* current = user_system->head;
    while (current != NULL) {
        if (strcmp(current->username, user_system->current_username) == 0) {
            current_user_type = current->type;
            break;
        }
        current = current->next;
    }

    // 显示菜单选项
    printf("\n----------------------------------------");
    printf("\n快递管理系统\n");
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER){
        printf(" 1. 入库包裹\n");
    }else{
        printf(" 1. 邮寄包裹\n");
    }
        
    printf(" 2. 查询用户包裹\n");
    printf(" 3. 取出包裹\n");
    printf(" 4. 拒收包裹\n");
    printf(" 5. 问题反馈\n");
    printf(" 6. 联系智能客服\n");
    printf(" 7. 修改账户密码\n");
    printf(" 8. 注销当前用户\n");
    printf(" 9. 好友系统\n");
    printf("10. 代取好友包裹\n");
    // 管理员和快递员可见的额外选项
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf("11. 包裹寄出\n");
        printf("12. 标记异常包裹\n");
        printf("13. 处理滞留包裹\n");
        // 仅管理员可见的选项
        if (current_user_type == USER_ADMIN) {
            printf("14. 清空系统数据\n");
            printf("15. 注册快递员账户\n");
            printf("16. 打开货架管理系统\n");
            printf("17. 处理用户反馈信息\n");
        }
    }
    printf(" 0. 退出登录\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system) {
    printf("\n----------------------------------------");
    showNotificationCount(user_system, package_system);
    printf("\n----------------------------------------\n");
    printf("\t通知菜单\n");
    printf("1. 查看通知详情\n");
    printf("2. 进入主菜单\n");
    printf("0. 退出登录\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}

void displayFriendMenu(UserSystem* user_system) {
    printf("\n----------------------------------------");
    printf("\n好友系统\n");
    printf("1. 添加好友\n");
    printf("2. 查看好友列表\n");
    printf("3. 删除好友\n");
    printf("4. 查看我的邀请码\n");
    printf("0. 返回主菜单\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}

void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system) {
    printf("\n----------------------------------------");
    printf("\n货架管理系统\n");
    printf("\n----------------------------------------\n");
    printf("1. 概况统计\n");
    printf("2. 分货架统计\n");
    printf("3. 显示全部包裹\n");
    printf("0. 返回主菜单\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
    printf("货架的分配原则\n");
    printf("根据包裹类型和大小分配货架号，层数随机生成\n");
    printf("1-3号货架分别放置大中小号非冷藏包裹\n");
    printf("4号货架专门放置冷藏包裹\n");
    printf("5号货架专门放置贵重包裹");
    printf("\n----------------------------------------\n");
}