
#ifndef FEEDBACK_ASSISTANT_H
#define FEEDBACK_ASSISTANT_H
#include "all_h_files.h"
#include "package_system.h"
#include "shelf_management.h"
#include "user_system.h"
#define MAX_FEEDBACK_LENGTH 500   // 反馈内容最大长度
#define MAX_RESPONSE_LENGTH 1000  // 回复内容最大长度
typedef enum {
    FEEDBACK_PENDING,     // 待处理：反馈已提交但未开始处理
    FEEDBACK_PROCESSING,  // 处理中：反馈正在由客服处理
    FEEDBACK_RESOLVED,    // 已解决：反馈问题已得到解决
    FEEDBACK_CLOSED       // 已关闭：反馈已结束，不再处理
} FeedbackStatus;
typedef struct FeedbackNode {
    int feedback_id;                    // 反馈唯一标识符
    char username[50];                  // 提交反馈的用户名
    char content[MAX_FEEDBACK_LENGTH];  // 反馈内容
    FeedbackStatus status;              // 反馈当前状态
    time_t submit_time;                 // 提交时间戳
    struct FeedbackNode* next;          // 链表下一节点指针
} FeedbackNode;
typedef struct {
    FeedbackNode* head;    // 反馈链表头节点
    int next_feedback_id;  // 下一个可用的反馈ID（初始为1000）
} FeedbackSystem;
// 处理用户反馈的主要函数，允许用户提交和管理反馈信息,返回零值退出
int handleFeedbackMenuInput(PackageSystem* package_system,
                                     UserSystem* user_system);
// 处理用户与智能助手的交互，提供自动化的客服支持
void handleContactSmartAssistant(PackageSystem* package_system,
                                 UserSystem* user_system);
// 显示系统中所有反馈的列表，包括反馈状态和详细信息
void displayFeedbackList(PackageSystem* system);
// 清除系统中的所有反馈记录
void clearAllFeedback(FeedbackSystem* system);
// 从文件中加载反馈数据，初始化反馈系统
FeedbackSystem* loadFeedbackFromFile();
// 输出指定反馈节点的状态信息
void outputFeedbackStatus(FeedbackNode* node);
#endif  // FEEDBACK_ASSISTANT_H