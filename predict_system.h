#ifndef PREDICT_SYSTEM_H
#define PREDICT_SYSTEM_H
#include "all_h_files.h"
typedef enum { SUNNY, RAINY, WINDY, SNOWY } Weather;
static const char* Weathersystem[] = {"晴天", "雨天", "刮风", "雪天"};
typedef struct predict_system {
    Weather weather;
    int strenthregistration;             // 预测注册人数
    int strenthVIP;                      // 预测用户充值成为VIP数
    int strenthpackage;                  // 预测滞留包裹数
    int strenthwater;                    // 预测流水账单变化
} predict_system;                        // 预测下一天的数据结构体
predict_system* initialpredictSystem();  // 初始化预测系统函数
void registrationpredict(
    predict_system* system,
    UserSystem* user_system,
    FeedbackSystem*
        feedback);  // 对注册人数进行预估(决定因素：用户好友数，用户反馈数）
void VIPpredict(
    predict_system* system,
    UserSystem*
        user_system);  // 对注册成为VIP人数进行预估（决定因素：用户在快递系统进行的总操作数）
int packageandwaterpredict(
    PackageSystem* system,
    predict_system* predict_System,
    UserSystem*
        user_system);  // 对滞留包裹数和流水账单变化进行预估（决定因素：用户在快递系统进行的总操作数）
int handlepredictSystem(UserSystem* user_system,
                        PackageSystem* package_system,
                        FeedbackSystem* feedback);  // 处理预测系统函数
#endif
