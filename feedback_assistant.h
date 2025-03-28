
#ifndef FEEDBACK_ASSISTANT_H
#define FEEDBACK_ASSISTANT_H
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
#define MAX_FEEDBACK_LENGTH 500   // ����������󳤶�
#define MAX_RESPONSE_LENGTH 1000  // �ظ�������󳤶�
typedef enum {
    FEEDBACK_PENDING,     // �������������ύ��δ��ʼ����
    FEEDBACK_PROCESSING,  // �����У����������ɿͷ�����
    FEEDBACK_RESOLVED,    // �ѽ�������������ѵõ����
    FEEDBACK_CLOSED       // �ѹرգ������ѽ��������ٴ���
} FeedbackStatus;
typedef struct FeedbackNode {
    int feedback_id;                    // ����Ψһ��ʶ��
    char username[50];                  // �ύ�������û���
    char content[MAX_FEEDBACK_LENGTH];  // ��������
    FeedbackStatus status;              // ������ǰ״̬
    time_t submit_time;                 // �ύʱ���
    struct FeedbackNode* next;          // ������һ�ڵ�ָ��
} FeedbackNode;
typedef struct {
    FeedbackNode* head;    // ��������ͷ�ڵ�
    int next_feedback_id;  // ��һ�����õķ���ID����ʼΪ1000��
} FeedbackSystem;
// �����û���������Ҫ�����������û��ύ�͹�������Ϣ,������ֵ�˳�
int handleFeedbackMenuInput(PackageSystem* package_system,
                                     UserSystem* user_system);
// �����û����������ֵĽ������ṩ�Զ����Ŀͷ�֧��
void handleContactSmartAssistant(PackageSystem* package_system,
                                 UserSystem* user_system);
// ��ʾϵͳ�����з������б���������״̬����ϸ��Ϣ
void displayFeedbackList(PackageSystem* system);
// ���ϵͳ�е����з�����¼
void clearAllFeedback(FeedbackSystem* system);
// ���ļ��м��ط������ݣ���ʼ������ϵͳ
FeedbackSystem* loadFeedbackFromFile();
// ���ָ�������ڵ��״̬��Ϣ
void outputFeedbackStatus(FeedbackNode* node);
#endif  // FEEDBACK_ASSISTANT_H