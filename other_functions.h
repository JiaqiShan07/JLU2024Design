#ifndef ASSISTANT_FUNCTION_H
#define ASSISTANT_FUNCTION_H
#include "all_h_files.h"
void pauseAndClearConsole(int getchar_num);
float calculateDeliveryFee(float weight_kg,
                           int size_choice,
                           int urgency_choice,
                           int type_choice,
                           int storage_choice,
                           UserType user_type);
UserNode* getSpecificUser(UserSystem* user_system, char target_username[]);

// ����һ��5λ�����룬��λΪ�����д��ĸ������λΪ�������
char* generateInviteCode();
#endif