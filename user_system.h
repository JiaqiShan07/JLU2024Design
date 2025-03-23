#ifndef USER_SYSTEM_H
#define USER_SYSTEM_H
#include "all_h_files.h"
#define MAX_USERS 100                   // ϵͳ֧�ֵ�����û���
#define MAX_USERNAME_LENGTH 50          // �û�������󳤶�
#define MIN_USERNAME_PASSWORD_LENGTH 3  // �û���/�������С����
#define MAX_PASSWORD_LENGTH 50          // �������󳤶�
#define MAX_FRIENDS 10                  // ���ѵ��������
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
// ��ʼ���û�����ϵͳ
UserSystem* initUserSystem();
// ���ļ��м����û���Ϣ
int loadUsersFromFile(UserSystem* system, const char* filename);
// ���û���Ϣ���浽�ļ�
int saveUsersToFile(UserSystem* system, const char* filename);
// ע�����û�
char* registerUser(UserSystem* system,
                   const char* username,
                   const char* password,
                   UserType type);
// �û���¼��֤
int loginUser(UserSystem* system, const char* username, const char* password);
// �û��˳���¼
void logoutUser(UserSystem* system);
// ��������û�����
void clearAllUsers(UserSystem* system);
// �޸��û�����
int changeUserPassword(UserSystem* system, const char* username);
// ���������޸ĵ��û�����
void handleChangePassword(UserSystem* system);
// ����ɾ���û��˻�����
int handleDeleteUserAccount(UserSystem* user_system,
                            PackageSystem* package_system);
// ɾ���û����ĺ���
int deleteUser(UserSystem* user_system,
               PackageSystem* package_system,
               const char* username);
// �����û�ע����û�����
void handleUserRegister(UserSystem* user_system);
// �����û���¼���û�����
void handleUserLogin(UserSystem* user_system);
// ������Աע����û�����
void handleCourierRegister(UserSystem* user_system);
#endif