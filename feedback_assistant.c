#include "all_h_files.h"
#define FEEDBACK_FILE "feedback.txt"
// 智能客服关键词结构
typedef struct {
    char keyword[50];                    // 关键词
    char response[MAX_RESPONSE_LENGTH];  // 回复内容
} KeywordResponse;
// 预设的智能客服回复
static const KeywordResponse AUTO_RESPONSES[] = {
    {"你好", "您好！我是AI助手，有什么我可以帮助您的吗？"},
    {"早上好", "早上好！祝您今天心情愉快！有什么我可以帮您的吗？"},
    {"下午好", "下午好！希望您度过了愉快的上午。有什么需要我帮助的吗？"},
    {"晚上好", "晚上好！今天过得怎么样？有什么我可以帮您的吗？"},
    {"再见", "感谢您的使用，祝您生活愉快！欢迎随时找我咨询问题。"},
    {"快递",
     "您好！关于快递问题，您可以通过系统的查询功能查看快递状态，或联系快递员了"
     "解详情。"},
    {"取件",
     "请您在快递到达后，使用系统的取件功能进行操作，并按照提示完成取件。"},
    {"存入", "请您使用系统的存入包裹功能进行操作，并按照提示完成操作。"},
    {"查询",
     "请您在快递到达后，使用系统的查询功能进行操作，并按照提示完成查询，可以显"
     "示包裹所有信息。"},
    {"拒收",
     "请您在快递到达后，使用系统的拒收功能进行操作，并按照提示完成拒收，拒收后"
     "会自动邮寄回发货地址。"},
    {"投诉",
     "您的意见对我们很重要，请通过反馈功能详细描述您遇到的问题，我们会尽快处理"
     "。"},
    {"时间",
     "我们的快递服务时间为每天8:00-22:00，请在此时间段内进行取件和寄件操作。"},
    {"丢失",
     "如果您的快递出现丢失情况，请立即联系管理员或提交反馈，我们会优先处理。"},
    {"VIP", "如需成为VIP用户，请进入主菜单寻找选项。"},
    {"vip", "如需成为VIP用户，请进入主菜单寻找选项。"},
    {"运费",
     "快递费用根据包裹重量和配送距离计算，您可以在寄件时查看具体费用。"},
    {"快递员",
     "如需联系快递员，请在系统中查看包裹详情，那里有快递员的联系方式。"},
    {"签收", "收到包裹后，请及时在系统中确认签收，这样可以保护您的权益。"},
    {"催单",
     "如果您的包裹延迟，可以使用系统的催单功能，我们会优先处理您的订单。"},
    {"保价", "寄件时可以选择保价服务，系统会根据物品价值收取相应的保价费用。"},
    {"破损",
     "如果发现包裹破损，请拍照保留证据，并立即通过反馈功能向我们反映。"},
    {"退款",
     "符合退款条件的订单，可以在系统中申请退款，我们会在3个工作日内处理。"},
    {"投递",
     "快递员会按照您填写的地址进行投递，如需修改地址请尽早在系统中操作。"},
    {"上门",
     "如需预约上门取件服务，请提前在系统中预约，我们会安排快递员上门。"},
    {"联系方式",
     "您可以在包裹详情中查看快递员联系方式，或通过客服反馈功能联系我们。"},
    {"包裹状态", "您可以通过系统的查询功能实时查看包裹的当前状态和位置信息。"},
    {"包裹位置",
     "包裹的实时位置信息可以通过系统查询功能查看，如有异常请及时反馈。"},
    {"包裹重量", "系统会显示包裹的具体重量信息，这将影响运费的计算。"},
    {"包裹尺寸", "您可以在包裹详情中查看包裹的具体尺寸信息。"}};

// 初始化反馈系统
FeedbackSystem* initFeedbackSystem() {
    FeedbackSystem* system = (FeedbackSystem*)malloc(sizeof(FeedbackSystem));
    if (system == NULL)
        return NULL;
    system->head = NULL;
    system->next_feedback_id = 1000;
    system->feedback_count = 0;
    return system;
}
// 保存反馈到文件
static int saveFeedbackToFile(FeedbackSystem* system) {
    if (system == NULL)
        return 0;
    FILE* file = fopen(FEEDBACK_FILE, "wb");
    if (file == NULL)
        return 0;
    // 写入反馈数量
    fwrite(&system->feedback_count, sizeof(int), 1, file);
    // 写入所有反馈节点
    FeedbackNode* current = system->head;
    while (current != NULL) {
        fwrite(current, sizeof(FeedbackNode), 1, file);
        current = current->next;
    }
    fclose(file);
    return 1;
}
// 从文件加载反馈
FeedbackSystem* loadFeedbackFromFile() {
    FeedbackSystem* system = initFeedbackSystem();
    if (system == NULL)
        return NULL;

    FILE* file = fopen(FEEDBACK_FILE, "rb");
    if (file == NULL)
        return system;

    // 先读取反馈数量
    int feedback_count;
    if (fread(&feedback_count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return system;
    }
    system->feedback_count = feedback_count;

    // 读取所有反馈节点
    FeedbackNode temp;
    while (fread(&temp, sizeof(FeedbackNode), 1, file) == 1) {
        FeedbackNode* new_node = (FeedbackNode*)malloc(sizeof(FeedbackNode));
        if (new_node == NULL)
            continue;

        *new_node = temp;
        new_node->next = system->head;
        system->head = new_node;

        if (new_node->feedback_id >= system->next_feedback_id) {
            system->next_feedback_id = new_node->feedback_id + 1;
        }
    }

    fclose(file);
    return system;
}

// 处理用户反馈
int handleFeedbackMenuInput(PackageSystem* package_system,
                            UserSystem* user_system) {
    if (package_system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return 0;
    }

    // 获取当前用户信息

    char username[50];
    UserNode* current = user_system->head;
    while (current != NULL) {
        // 找到当前登录的用户
        if (strcmp(current->username, user_system->current_username) == 0) {
            strcpy(username, current->username);
            break;
        }
        current = current->next;
    }

    // 加载反馈系统
    FeedbackSystem* feedback_system = loadFeedbackFromFile();
    if (feedback_system == NULL) {
        printf("反馈系统初始化失败\n");
        return 0;
    }
    int choice = getValidatedIntegerInput(0, 2, 1);
    switch (choice) {
        case 1: {
            printf("\n请输入反馈内容（最多%d字符）:\n",
                   MAX_FEEDBACK_LENGTH - 1);
            char feedback_content[MAX_FEEDBACK_LENGTH];
            getValidatedStringInput(feedback_content, MAX_FEEDBACK_LENGTH);

            FeedbackNode* new_feedback =
                (FeedbackNode*)malloc(sizeof(FeedbackNode));
            if (new_feedback == NULL) {
                printf("内存分配失败\n");
                break;
            }

            new_feedback->feedback_id = feedback_system->next_feedback_id++;
            strncpy(new_feedback->username, username,
                    sizeof(new_feedback->username));
            strncpy(new_feedback->content, feedback_content,
                    sizeof(new_feedback->content));
            new_feedback->status = FEEDBACK_PENDING;
            new_feedback->submit_time = time(NULL);
            new_feedback->next = feedback_system->head;
            feedback_system->head = new_feedback;
            feedback_system->feedback_count++;
            if (saveFeedbackToFile(feedback_system)) {
                printf("----------------------------------------\n");
                printf("反馈提交成功！反馈ID：%04d\n",
                       new_feedback->feedback_id);
            } else {
                printf("反馈保存失败\n");
            }
            pauseAndClearConsole(1);
            break;
        }
        case 2: {
            printf("\n您的反馈记录：\n");
            printf("----------------------------------------\n");
            int found = 0;
            FeedbackNode* current = feedback_system->head;
            while (current != NULL) {
                if (strcmp(current->username, username) == 0) {
                    found = 1;
                    printf("反馈ID: %04d\n", current->feedback_id);
                    printf("内容: %s\n", current->content);
                    outputFeedbackStatus(current);
                    printf("提交时间: %s", ctime(&current->submit_time));
                    printf("----------------------------------------\n");
                }
                current = current->next;
            }

            if (!found) {
                printf("----------------------------------------\n");
                printf("暂无反馈记录\n");
                printf("----------------------------------------\n");
            }
            pauseAndClearConsole(1);
            break;
        }

        case 0:
            printf("----------------------------------------\n");
            printf("返回主菜单\n");
            printf("----------------------------------------\n");

            return 0;
            break;

        default:
            printf("----------------------------------------\n");
            printf("无效的选择，请重试\n");
            printf("----------------------------------------\n");
            break;
    }
    return 1;
}

// 处理智能客服对话
void handleContactSmartAssistant(PackageSystem* package_system,
                                 UserSystem* user_system) {
    if (package_system == NULL || user_system == NULL) {
        printf("系统未初始化\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("智能客服助手\n");
    printf("您可以询问关于快递、取件、投诉等问题\n");
    printf("输入'退出'结束对话\n");
    printf("----------------------------------------\n");

    char input[MAX_FEEDBACK_LENGTH];
    while (1) {
        printf("\n请输入您的问题: ");
        getValidatedStringInput(input, MAX_FEEDBACK_LENGTH);

        if (strcmp(input, "退出") == 0) {
            printf("----------------------------------------\n");
            printf("感谢您的使用，再见！\n");
            printf("----------------------------------------\n");
            Sleep(2000);  // 暂停2秒
            pauseAndClearConsole(0);
            break;
        }

        // 查找关键词并回复
        // 标记是否找到匹配的关键词回复
        int found_response = 0;

        // 遍历预设的关键词回复数组
        // sizeof(AUTO_RESPONSES)获取整个数组的大小
        // sizeof(AUTO_RESPONSES[0])获取单个元素的大小
        // 两者相除得到数组元素个数
        for (size_t i = 0;
             i < sizeof(AUTO_RESPONSES) / sizeof(AUTO_RESPONSES[0]); i++) {
            // 使用strstr函数检查用户输入中是否包含当前关键词
            // strstr返回NULL表示未找到，非NULL表示找到
            if (strstr(input, AUTO_RESPONSES[i].keyword) != NULL) {
                printf("响应中...\n");
                Sleep(1000);  // 暂停1.5秒
                // 找到匹配的关键词后输出对应的预设回复
                printf("\n智能助手: %s\n", AUTO_RESPONSES[i].response);
                // 设置标记表示已找到回复
                found_response = 1;
                // 找到第一个匹配的关键词就退出循环
                break;
            }
        }
        // 只有在用户实际输入内容且未找到匹配关键词时才显示服务器繁忙
        if (!found_response && strlen(input) > 0) {
            Sleep(1000);  // 暂停2秒
            printf("\n服务器繁忙，请稍后再试！\n");
            printf("思考中...\n");
            Sleep(1000);  // 暂停3秒
            printf(
                "\n智能助手: "
                "抱歉，我暂时无法理解您的问题。您可以尝试询问关于快递、取件、投"
                "诉等具体问题，或通过反馈功能提交您的问题。\n");
        }
    }
}
// 集成了展示反馈菜单和处理反馈菜单的功能
void displayFeedbackListAndHandleInput(PackageSystem* system) {
    // 加载反馈系统
    FeedbackSystem* feedback_system = loadFeedbackFromFile();
    if (feedback_system == NULL) {
        printf("反馈系统初始化失败\n");
        return;
    }
    while (1) {
        printf("\n----------------------------------------\n");
        printf("反馈信息列表\n");
        printf("----------------------------------------\n");
        printf("%-8s %-15s %-20s %-10s\n", "反馈ID", "用户名", "提交时间",
               "状态");
        printf("----------------------------------------\n");

        // 显示所有反馈的基本信息
        FeedbackNode* current = feedback_system->head;
        while (current != NULL) {
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M",
                     localtime(&current->submit_time));
            printf("%-8d %-15s %-20s %-10s\n", current->feedback_id,
                   current->username, time_str,
                   current->status == FEEDBACK_PENDING      ? "待处理"
                   : current->status == FEEDBACK_PROCESSING ? "处理中"
                   : current->status == FEEDBACK_RESOLVED   ? "已解决"
                                                            : "已关闭");
            current = current->next;
        }

        printf("\n输入反馈ID查看详细信息（输入0返回）: ");
        int feedback_id = getValidatedIntegerInput(1000, 9999, 1);

        if (feedback_id == 0) {
            break;
        }

        // 查找并显示指定ID的反馈详细信息
        current = feedback_system->head;
        int found = 0;
        while (current != NULL) {
            if (current->feedback_id == feedback_id) {
                found = 1;
                printf("\n----------------------------------------\n");
                printf("反馈详细信息\n");
                printf("----------------------------------------\n");
                printf("反馈ID: %d\n", current->feedback_id);
                printf("用户名: %s\n", current->username);
                printf("提交时间: %s", ctime(&current->submit_time));
                outputFeedbackStatus(current);
                printf("反馈内容:\n%s\n", current->content);
                printf("----------------------------------------\n");
                printf("---------------可选的操作-----------------\n");
                printf(
                    "1. 标记为待处理\n2. 标记为处理中\n3. 标记为已解决\n4. "
                    "标记为已关闭\n0. 返回上一级\n请选择操作: ");
                printf("----------------------------------------\n");
                int status_choice = getValidatedIntegerInput(0, 4, 1);

                if (status_choice > 0) {
                    switch (status_choice) {
                        case 1:
                            current->status = FEEDBACK_PENDING;
                            break;
                        case 2:
                            current->status = FEEDBACK_PROCESSING;
                            break;
                        case 3:
                            current->status = FEEDBACK_RESOLVED;
                            break;
                        case 4:
                            current->status = FEEDBACK_CLOSED;
                            break;
                    }
                    if (saveFeedbackToFile(feedback_system)) {
                        printf("\n反馈状态已更新\n");
                    } else {
                        printf("\n状态更新失败\n");
                    }
                    pauseAndClearConsole(1);
                }
                break;
            }
            current = current->next;
        }

        if (!found) {
            printf("未找到ID为%d的反馈信息\n", feedback_id);
        }
        pauseAndClearConsole(0);
    }
}
void clearAllFeedback(FeedbackSystem* system) {
    if (system == NULL) {
        return;
    }
    FeedbackNode* current = system->head;
    while (current) {
        FeedbackNode* temp = current;
        current = current->next;
        free(temp);
    }
    system->head = NULL;
    system->next_feedback_id = 1000;
    FILE* file = fopen(FEEDBACK_FILE, "wb");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("清空反馈失败\n");
    }
}
void outputFeedbackStatus(FeedbackNode* node) {
    const char* status_str;
    FeedbackNode* current = node;
    if (current->status == FEEDBACK_PENDING) {
        status_str = "待处理";
    } else if (current->status == FEEDBACK_PROCESSING) {
        status_str = "处理中";
    } else if (current->status == FEEDBACK_RESOLVED) {
        status_str = "已解决";
    } else {
        status_str = "已关闭";
    }
    printf("状态: %s\n", status_str);
}