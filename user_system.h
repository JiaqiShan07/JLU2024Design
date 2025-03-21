#ifndef USER_SYSTEM_H
#define USER_SYSTEM_H
#include "all_h_files.h"
#define MAX_USERS 100           // ϵͳ֧�ֵ�����û���
#define MAX_USERNAME_LENGTH 50  // �û�������󳤶�
#define MAX_PASSWORD_LENGTH 50  // �������󳤶�
#define MAX_FRIENDS 10          // ���ѵ��������
typedef struct UserNode {
    char username[MAX_USERNAME_LENGTH];              // �û���
    char password[MAX_PASSWORD_LENGTH];              // ����
    int friend_count;                                // ��������
    char friends[MAX_FRIENDS][MAX_USERNAME_LENGTH];  // �����û�������
    char invateCode[10];                             // ������[10]
    UserType type;                                   // �û�����
    struct UserNode* next;                           // ָ����һ���û��ڵ��ָ��
} UserNode;
typedef struct {
    UserNode* head;                              // �û������ͷָ��
    int user_count;                              // ϵͳ�е��û�����
    char current_username[MAX_USERNAME_LENGTH];  // ��ǰ��¼�û����û���
    bool is_login;                               // ��¼״̬
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