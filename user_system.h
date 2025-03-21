#ifndef USER_SYSTEM_H
#define USER_SYSTEM_H
#include "all_h_files.h"
#define MAX_USERS 100           // 系统支持的最大用户数
#define MAX_USERNAME_LENGTH 50  // 用户名的最大长度
#define MAX_PASSWORD_LENGTH 50  // 密码的最大长度
#define MAX_FRIENDS 10          // 好友的最大数量
typedef struct UserNode {
    char username[MAX_USERNAME_LENGTH];              // 用户名
    char password[MAX_PASSWORD_LENGTH];              // 密码
    int friend_count;                                // 好友数量
    char friends[MAX_FRIENDS][MAX_USERNAME_LENGTH];  // 好友用户名数组
    char invateCode[10];                             // 邀请码[10]
    UserType type;                                   // 用户类型
    struct UserNode* next;                           // 指向下一个用户节点的指针
} UserNode;
typedef struct {
    UserNode* head;                              // 用户链表的头指针
    int user_count;                              // 系统中的用户数量
    char current_username[MAX_USERNAME_LENGTH];  // 当前登录用户的用户名
    bool is_login;                               // 登录状态
} UserSystem;
UserSystem* initUserSystem();
int loadUsersFromFile(UserSystem* system, const char* filename);
int saveUsersToFile(UserSystem* system, const char* filename);
char* registerUser(UserSystem* system,
                   const char* username,
                   const char* password,
                   UserType type);
int loginUser(UserSystem* system, const char* username, const char* password);
void logoutUser(UserSystem* system);
void clearAllUsers(UserSystem* system);
int changeUserPassword(UserSystem* system, const char* username);
void handleChangePassword(UserSystem* system);
int deleteUserAccount(UserSystem* system);
void handleUserRegister(UserSystem* user_system);
void handleUserLogin(UserSystem* user_system);
void handleCourierRegister(UserSystem* user_system);
#endif