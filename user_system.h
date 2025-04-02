#ifndef USER_SYSTEM_H
#define USER_SYSTEM_H
#include "all_h_files.h"
#define MAX_USERS 100                    // 系统支持的最大用户数
#define MAX_USERNAME_LENGTH 50          // 用户名的最大长度
#define MIN_USERNAME_PASSWORD_LENGTH 3  // 用户名/密码的最小长度
#define MAX_PASSWORD_LENGTH 50          // 密码的最大长度
#define MAX_FRIENDS 10                  // 好友的最大数量
typedef struct UserNode {
    char username[MAX_USERNAME_LENGTH];              // 用户名
    char password[MAX_PASSWORD_LENGTH];              // 密码
    int friend_count;                                // 好友数量
    char friends[MAX_FRIENDS][MAX_USERNAME_LENGTH];  // 好友用户名数组
    char invateCode[10];                             // 邀请码[10]
    UserType type;                                   // 用户类型
    struct UserNode* next;                           // 指向下一个用户节点的指针
    time_t VIPtime;                                  // 用户成为VIP时间
    time_t looktime[1000];                          // 管理员查看交易流水记录
    int adminchoice[1000];                          // 记录管理员每次的选项
    int searchcount;                                // 辅助变量
    int packagepre;                                 // 统计每名用户进行的包裹操作数
} UserNode;
typedef struct {
    UserNode* head;                              // 用户链表的头指针
    int user_count;                              // 系统中的用户数量
    char current_username[MAX_USERNAME_LENGTH];  // 当前登录用户的用户名
    bool is_login;                               // 登录状态
} UserSystem;
// 初始化用户管理系统
UserSystem* initUserSystem();
// 从文件中加载用户信息
int loadUsersFromFile(UserSystem* system, const char* filename);
// 将用户信息保存到文件
int saveUsersToFile(UserSystem* system, const char* filename);
// 注册新用户
char* registerUser(UserSystem* system,
                   const char* username,
                   const char* password,
                   UserType type);
// 用户登录验证
int loginUser(UserSystem* system, const char* username, const char* password);
// 用户退出登录
void logoutUser(UserSystem* system);
// 清空所有用户数据
void clearAllUsers(UserSystem* system);
// 修改用户密码
int changeUserPassword(UserSystem* system, const char* username);
// 处理密码修改的用户交互
void handleChangePassword(UserSystem* system);
// 处理删除用户账户交互
int handleDeleteUserAccount(UserSystem* user_system,
                            PackageSystem* package_system);
// 删除用户核心函数
int deleteUser(UserSystem* user_system,
               PackageSystem* package_system,
               const char* username);
// 处理用户注册的用户交互
void handleUserRegister(UserSystem* user_system);
// 处理用户登录的用户交互
void handleUserLogin(UserSystem* user_system);
// 处理快递员注册的用户交互
void handleCourierRegister(UserSystem* user_system);
// 处理升级VIP用户的交互
void handleUpgradeVIP(UserSystem* user_system);
// 升级VIP的核心函数
int upgradeVIP(UserSystem* user_system);
// 记录VIP流水函数
int VIPwater(UserSystem* user_system);
// 记录用户寄包裹流水函数
int packagewater(UserSystem* user_system, PackageSystem* system);
// 处理管理员浏览流水记录的交互
void handleAdminLookWaterLog(UserSystem* user_system, PackageSystem* system);
// 记录用户寄件流水信息
int packagewater(UserSystem* user_system, PackageSystem* system);
// 记录用户VIP充值流水信息
int VIPwater(UserSystem* user_system);
// 处理用户升级VIP的交互界面
void handleUpgradeVIP(UserSystem* user_system);
// 执行用户升级VIP的核心功能
int upgradeVIP(UserSystem* user_system);
#endif