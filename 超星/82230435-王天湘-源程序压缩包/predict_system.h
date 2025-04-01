#ifndef PREDICT_SYSTEM_H
#define PREDICT_SYSTEM_H
#include "all_h_files.h"
typedef enum { SUNNY, RAINY, WINDY, SNOWY } Weather;
static const char* Weathersystem[] = {"����", "����", "�η�", "ѩ��"};
typedef struct predict_system {
    Weather weather;
    int strenthregistration;             // Ԥ��ע������
    int strenthVIP;                      // Ԥ���û���ֵ��ΪVIP��
    int strenthpackage;                  // Ԥ������������
    int strenthwater;                    // Ԥ����ˮ�˵��仯
} predict_system;                        // Ԥ����һ������ݽṹ��
predict_system* initialpredictSystem();  // ��ʼ��Ԥ��ϵͳ����
void registrationpredict(
    predict_system* system,
    UserSystem* user_system,
    FeedbackSystem*
        feedback);  // ��ע����������Ԥ��(�������أ��û����������û���������
void VIPpredict(
    predict_system* system,
    UserSystem*
        user_system);  // ��ע���ΪVIP��������Ԥ�����������أ��û��ڿ��ϵͳ���е��ܲ�������
int packageandwaterpredict(
    PackageSystem* system,
    predict_system* predict_System,
    UserSystem*
        user_system);  // ����������������ˮ�˵��仯����Ԥ�����������أ��û��ڿ��ϵͳ���е��ܲ�������
int handlepredictSystem(UserSystem* user_system,
                        PackageSystem* package_system,
                        FeedbackSystem* feedback);  // ����Ԥ��ϵͳ����
#endif
