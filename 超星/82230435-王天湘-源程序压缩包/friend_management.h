#pragma once
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
// 处理添加好友的用户交互功能，包括输入好友用户名和邀请码
void handleAddFriends(UserSystem* uer_system);
// 执行添加好友操作，验证邀请码并建立好友关系
void addFriends(UserSystem* user_system,
                const char* friend_username,
                const char* invite_code);
// 处理删除好友的用户交互功能，允许用户选择要删除的好友
void handleRemoveFriends(UserSystem* user_system);
// 显示当前用户的所有好友列表
void handleViewFriends(UserSystem* user_system);
// 显示当前用户的邀请码，用于添加好友时的验证
void handleViewInviteCode(UserSystem* user_system);
// 从用户的好友列表中删除指定的好友
int deleteFriend(UserSystem* user_system, const char* friend_username);