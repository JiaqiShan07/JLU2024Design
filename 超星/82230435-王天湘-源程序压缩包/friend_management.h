#pragma once
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
// ������Ӻ��ѵ��û��������ܣ�������������û�����������
void handleAddFriends(UserSystem* uer_system);
// ִ����Ӻ��Ѳ�������֤�����벢�������ѹ�ϵ
void addFriends(UserSystem* user_system,
                const char* friend_username,
                const char* invite_code);
// ����ɾ�����ѵ��û��������ܣ������û�ѡ��Ҫɾ���ĺ���
void handleRemoveFriends(UserSystem* user_system);
// ��ʾ��ǰ�û������к����б�
void handleViewFriends(UserSystem* user_system);
// ��ʾ��ǰ�û��������룬������Ӻ���ʱ����֤
void handleViewInviteCode(UserSystem* user_system);
// ���û��ĺ����б���ɾ��ָ���ĺ���
int deleteFriend(UserSystem* user_system, const char* friend_username);