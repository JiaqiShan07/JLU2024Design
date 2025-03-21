#pragma once
#include"all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
void handleAddFriends(UserSystem* uer_system);
void addFriends(UserSystem* user_system,
                const char* friend_username,
                const char* invite_code);
void handleRemoveFriends(UserSystem* user_system);
void handleViewFriends(UserSystem* user_system);
void handleViewInviteCode(UserSystem* user_system);
void deleteFriend(UserSystem* user_system, const char* friend_username);