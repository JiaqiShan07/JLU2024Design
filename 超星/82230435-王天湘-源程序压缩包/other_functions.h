#ifndef ASSISTANT_FUNCTION_H
#define ASSISTANT_FUNCTION_H
#include "all_h_files.h"
// 暂停程序执行并清除控制台显示，参数指定需要读取的字符数
void pauseAndClearConsole(int getchar_num);
// 根据包裹重量、尺寸、紧急程度、类型、存储要求和用户类型计算配送费用
float calculateDeliveryFee(float weight_kg,
                           int size_choice,
                           int urgency_choice,
                           int type_choice,
                           int storage_choice,
                           UserType user_type);
// 根据用户名在系统中查找并返回指定用户节点
UserNode* getSpecificUser(UserSystem* user_system, char target_username[]);

// 生成一个5位邀请码，首位为随机大写字母，其余位为随机数字
char* generateInviteCode();
#endif