#include "all_h_files.h"
// 显示登录菜单
// 打印系统登录界面，包括用户注册、登录和退出选项
void displayLoginMenu() {
    printf("----------欢迎使用快递管理系统--------------\n");
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
    printf("-----------------主菜单--------------------\n");
    printf("---------------收寄件系统------------------\n");
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf(" 1. 入库包裹\n");
    } else {
        printf(" 1. 邮寄包裹\n");
    }

    printf(" 2. 查询用户包裹\n");
    printf(" 3. 取出包裹\n");
    printf(" 4. 拒收包裹\n");
    printf("--------------好友管理系统----------------\n");
    printf(" 5. 好友系统\n");
    printf(" 6. 代取好友包裹\n");
    printf("------------------其它-------------------\n");
    printf(" 7. 问题反馈\n");
    printf(" 8. 联系智能助手\n");
    printf("---------------个人管理系统--------------\n");
    printf(" 9. 修改账户密码\n");
    printf("10. 账户注销\n");
    // 除VIP用户可见的其他选项
    if ((current_user_type != USER_ADMIN) &&
        (current_user_type != USER_COURIER) &&
        (current_user_type != USER_VIP)) {
        printf("11. 升级成为VIP\n");
        printf("----------------------------------------\n");
    }
    if (current_user_type == USER_VIP) {
        printf("----------------------------------------\n");
    }
    // 管理员和快递员可见的额外选项
    if (current_user_type == USER_ADMIN || current_user_type == USER_COURIER) {
        printf("----------寄出包裹及异常处理系统----------\n");
        printf("11. 包裹寄出\n");
        printf("12. 标记异常包裹\n");
        printf("13. 处理滞留包裹\n");
        // 仅管理员可见的选项
        if (current_user_type == USER_ADMIN) {
            printf("14. 处理无用包裹记录\n");
            printf("------------管理员系统管理---------------\n");
            printf("15. 注册快递员账户\n");
            printf("16. 打开货架管理系统\n");
            printf("17. 处理用户反馈信息\n");
            printf("18. 清除系统数据\n");
            printf("19. 查看系统流水日志\n");
            printf("20. 预测快递驿站第二天发展趋势\n");
            printf("----------------------------------------\n");
        }
    }
    printf(" 0. 退出登录\n");
    printf("请选择操作: ");
}
void displayNotificationMenu(UserSystem* user_system,
                             PackageSystem* package_system) {
    printf("\n----------------------------------------");
    showNotificationCount(user_system, package_system);
    printf("\n--------------通知菜单-------------------\n");
    printf("1. 查看通知详情\n");
    printf("2. 进入主菜单\n");
    printf("0. 退出登录\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}
void displaysecondNotificationMenu(UserSystem* user_system,
                                   PackageSystem* package_system) {
    printf("\n-------------分类通知详情--------------\n");
    printf("1. 取件通知  (%d条新通知)\n",
           getDifferentTypePackageCount(user_system, package_system,
                                        PENDING_PICKUP));
    printf("2. 待寄出通知(%d条新通知)\n",
           getDifferentTypePackageCount(user_system, package_system,
                                        PENDING_DELIVERY));
    printf("3. 异常通知  (%d条新通知)\n",
           getDifferentTypePackageCount(user_system, package_system, ABNORMAL));
    printf("4. 滞留通知  (%d条新通知)\n",
           getDifferentTypePackageCount(user_system, package_system, STRANDED));
    printf("5. 拒收通知  (%d条新通知)\n",
           getDifferentTypePackageCount(user_system, package_system, REJECTED));
    printf("0. 退出通知详情\n");
    printf("----------------------------------------\n");
    printf("请选择操作: ");
}

void displayFriendMenu(UserSystem* user_system) {
    printf("------------------好友系统-----------------\n");
    printf("1. 添加好友\n");
    printf("2. 查看好友列表\n");
    printf("3. 删除好友\n");
    printf("4. 查看我的邀请码\n");
    printf("0. 返回主菜单\n");
    printf("请选择操作: ");
    printf("\n----------------------------------------\n");
}

void displayShelfMenu(UserSystem* user_system, PackageSystem* package_system) {
    printf("----------------货架管理系统---------------\n");
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
// 展示管理员日志子系统的菜单
void displayAdminLogMenu() {
    printf("-------------流水日志菜单-----------------\n");
    printf("请选择您要查看的流水项目:\n");
    printf("1.用户充值VIP流水\n");
    printf("2.用户包裹费用流水\n");
    printf("3.管理员浏览流水记录\n");
    printf("0.返回\n");
    printf("----------------------------------------\n");
    printf("请选择 (0-3): ");
}
void displayFeedbackMenu() {
    printf("\n----------------反馈系统---------------\n");
    printf("反馈系统\n");
    printf("1. 提交新反馈\n");
    printf("2. 查看我的反馈\n");
    printf("0. 返回主菜单\n");
    printf("----------------------------------------\n");
    printf("请选择操作:(0-2) ");
}
void displayPredictMenu() {
    printf("正在根据往期数据为您预测......\n");
    printf("-------------超级无敌预测系统-------------\n");
    printf("下面根据第二天天气情况对包裹滞留数目的预测\n");
    printf("处理选项:\n");
    printf("1. 晴天\n");
    printf("2. 雨天\n");
    printf("3. 刮风\n");
    printf("4. 雪天\n");
    printf("0. 返回主菜单\n");
    printf("----------------------------------------\n");
    printf("请确定第二天的天气状况：(1-4)\n");
}