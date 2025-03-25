#include "all_h_files.h"
#define FEEDBACK_FILE "feedback.txt"
// ���ܿͷ��ؼ��ʽṹ
typedef struct {
    char keyword[50];                    // �ؼ���
    char response[MAX_RESPONSE_LENGTH];  // �ظ�����
} KeywordResponse;
// Ԥ������ܿͷ��ظ�
static const KeywordResponse AUTO_RESPONSES[] = {
    {"���", "���ã�����AI���֣���ʲô�ҿ��԰���������"},
    {"���",
     "���ã����ڿ�����⣬������ͨ��ϵͳ�Ĳ�ѯ���ܲ鿴���״̬������ϵ���Ա��"
     "�����顣"},
    {"ȡ��",
     "�����ڿ�ݵ����ʹ��ϵͳ��ȡ�����ܽ��в�������������ʾ���ȡ����"},
    {"����", "����ʹ��ϵͳ�Ĵ���������ܽ��в�������������ʾ��ɲ�����"},
    {"��ѯ",
     "�����ڿ�ݵ����ʹ��ϵͳ�Ĳ�ѯ���ܽ��в�������������ʾ��ɲ�ѯ��������"
     "ʾ����������Ϣ��"},
    {"����",
     "�����ڿ�ݵ����ʹ��ϵͳ�ľ��չ��ܽ��в�������������ʾ��ɾ��գ����պ�"
     "���Զ��ʼĻط�����ַ��"},
    {"Ͷ��",
     "������������Ǻ���Ҫ����ͨ������������ϸ���������������⣬���ǻᾡ�촦��"
     "��"},
    {"ʱ��",
     "���ǵĿ�ݷ���ʱ��Ϊÿ��8:00-22:00�����ڴ�ʱ����ڽ���ȡ���ͼļ�������"},
    {"��ʧ",
     "������Ŀ�ݳ��ֶ�ʧ�������������ϵ����Ա���ύ���������ǻ����ȴ���"},
    {"VIP",
     "�����ΪVIP�û�����������˵�Ѱ��ѡ�"},
    {"vip",
     "�����ΪVIP�û�����������˵�Ѱ��ѡ�"}
};

// ��ʼ������ϵͳ
static FeedbackSystem* initFeedbackSystem() {
    FeedbackSystem* system = (FeedbackSystem*)malloc(sizeof(FeedbackSystem));
    if (system == NULL)
        return NULL;

    system->head = NULL;
    system->next_feedback_id = 1000;
    return system;
}
// ���淴�����ļ�
static int saveFeedbackToFile(FeedbackSystem* system) {
    if (system == NULL)
        return 0;

    FILE* file = fopen(FEEDBACK_FILE, "wt");
    if (file == NULL)
        return 0;

    FeedbackNode* current = system->head;
    while (current != NULL) {
        fwrite(current, sizeof(FeedbackNode), 1, file);
        current = current->next;
    }

    fclose(file);
    return 1;
}

// ���ļ����ط���
FeedbackSystem* loadFeedbackFromFile() {
    FeedbackSystem* system = initFeedbackSystem();
    if (system == NULL)
        return NULL;

    FILE* file = fopen(FEEDBACK_FILE, "rt");
    if (file == NULL)
        return system;

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

// �����û�����
void handleFeedback(PackageSystem* package_system, UserSystem* user_system) {
    if (package_system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    // ��ȡ��ǰ�û���Ϣ

    char username[50];
    UserNode* current = user_system->head;
    while (current != NULL) {
        // �ҵ���ǰ��¼���û�
        if (strcmp(current->username, user_system->current_username) == 0) {
            strcpy(username, current->username);
            break;
        }
        current = current->next;
    }

    // ���ط���ϵͳ
    FeedbackSystem* feedback_system = loadFeedbackFromFile();
    if (feedback_system == NULL) {
        printf("����ϵͳ��ʼ��ʧ��\n");
        return;
    }

    int choice;
    do {
        printf("\n----------------------------------------\n");
        printf("����ϵͳ\n");
        printf("1. �ύ�·���\n");
        printf("2. �鿴�ҵķ���\n");
        printf("0. �������˵�\n");
        printf("----------------------------------------\n");
        printf("��ѡ�����: ");
        choice = getValidatedIntegerInput(0, 2, 1);
        printf("----------------------------------------\n");

        switch (choice) {
            case 1: {
                printf("\n�����뷴�����ݣ����%d�ַ���:\n",
                       MAX_FEEDBACK_LENGTH - 1);
                char feedback_content[MAX_FEEDBACK_LENGTH];
                getValidatedStringInput(feedback_content, MAX_FEEDBACK_LENGTH);

                FeedbackNode* new_feedback =
                    (FeedbackNode*)malloc(sizeof(FeedbackNode));
                if (new_feedback == NULL) {
                    printf("�ڴ����ʧ��\n");
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

                if (saveFeedbackToFile(feedback_system)) {
                    printf("----------------------------------------\n");
                    printf("�����ύ�ɹ�������ID��%04d\n",
                           new_feedback->feedback_id);
                } else {
                    printf("��������ʧ��\n");
                }
                pauseAndClearConsole(1);
                break;
            }

            case 2: {
                printf("\n���ķ�����¼��\n");
                printf("----------------------------------------\n");
                int found = 0;
                FeedbackNode* current = feedback_system->head;

                while (current != NULL) {
                    if (strcmp(current->username, username) == 0) {
                        found = 1;
                        printf("����ID: %04d\n", current->feedback_id);
                        printf("����: %s\n", current->content);
                        outputFeedbackStatus(current);
                        printf("�ύʱ��: %s", ctime(&current->submit_time));
                        printf("----------------------------------------\n");
                    }
                    current = current->next;
                }

                if (!found) {
                    printf("----------------------------------------\n");
                    printf("���޷�����¼\n");
                    printf("----------------------------------------\n");
                }
                pauseAndClearConsole(1);
                break;
            }

            case 0:
                printf("----------------------------------------\n");
                printf("�������˵�\n");
                printf("----------------------------------------\n");
                break;

            default:
                printf("----------------------------------------\n");
                printf("��Ч��ѡ��������\n");
                printf("----------------------------------------\n");
                break;
        }
    } while (choice != 0);

    // ������ϵͳ
    FeedbackNode* current_feedback = feedback_system->head;
    while (current_feedback != NULL) {
        FeedbackNode* temp = current_feedback;
        current_feedback = current_feedback->next;
        free(temp);
    }
    free(feedback_system);
}

// �������ܿͷ��Ի�
void handleContactSmartAssistant(PackageSystem* package_system,
                                 UserSystem* user_system) {
    if (package_system == NULL || user_system == NULL) {
        printf("ϵͳδ��ʼ��\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("���ܿͷ�����\n");
    printf("������ѯ�ʹ��ڿ�ݡ�ȡ����Ͷ�ߵ�����\n");
    printf("����'�˳�'�����Ի�\n");
    printf("----------------------------------------\n");

    char input[MAX_FEEDBACK_LENGTH];
    while (1) {
        printf("\n��������������: ");
        getValidatedStringInput(input, MAX_FEEDBACK_LENGTH);

        if (strcmp(input, "�˳�") == 0) {
            printf("----------------------------------------\n");
            printf("��л����ʹ�ã��ټ���\n");
            printf("----------------------------------------\n");
            Sleep(2000);  // ��ͣ2��
            pauseAndClearConsole(0);
            break;
        }

        // ���ҹؼ��ʲ��ظ�
        // ����Ƿ��ҵ�ƥ��Ĺؼ��ʻظ�
        int found_response = 0;

        // ����Ԥ��Ĺؼ��ʻظ�����
        // sizeof(AUTO_RESPONSES)��ȡ��������Ĵ�С
        // sizeof(AUTO_RESPONSES[0])��ȡ����Ԫ�صĴ�С
        // ��������õ�����Ԫ�ظ���
        for (size_t i = 0;
             i < sizeof(AUTO_RESPONSES) / sizeof(AUTO_RESPONSES[0]); i++) {
            // ʹ��strstr��������û��������Ƿ������ǰ�ؼ���
            // strstr����NULL��ʾδ�ҵ�����NULL��ʾ�ҵ�
            if (strstr(input, AUTO_RESPONSES[i].keyword) != NULL) {
                printf("��Ӧ��...\n");
                Sleep(1000);  // ��ͣ1.5��
                // �ҵ�ƥ��Ĺؼ��ʺ������Ӧ��Ԥ��ظ�
                printf("\n��������: %s\n", AUTO_RESPONSES[i].response);
                // ���ñ�Ǳ�ʾ���ҵ��ظ�
                found_response = 1;
                // �ҵ���һ��ƥ��Ĺؼ��ʾ��˳�ѭ��
                break;
            }
        }
        // ֻ�����û�ʵ������������δ�ҵ�ƥ��ؼ���ʱ����ʾ��������æ
        if (!found_response && strlen(input) > 0) {
            Sleep(1000);  // ��ͣ2��
            printf("\n��������æ�����Ժ����ԣ�\n");
            printf("˼����...\n");
            Sleep(1000);  // ��ͣ3��
            printf(
                "\n��������: "
                "��Ǹ������ʱ�޷�����������⡣�����Գ���ѯ�ʹ��ڿ�ݡ�ȡ����Ͷ"
                "�ߵȾ������⣬��ͨ�����������ύ�������⡣\n");
        }
    }
}
// ������չʾ�����˵��ʹ������˵��Ĺ���
void displayFeedbackList(PackageSystem* system) {
    // ���ط���ϵͳ
    FeedbackSystem* feedback_system = loadFeedbackFromFile();
    if (feedback_system == NULL) {
        printf("����ϵͳ��ʼ��ʧ��\n");
        return;
    }

    while (1) {
        printf("\n----------------------------------------\n");
        printf("������Ϣ�б�\n");
        printf("----------------------------------------\n");
        printf("%-8s %-15s %-20s %-10s\n", "����ID", "�û���", "�ύʱ��",
               "״̬");
        printf("----------------------------------------\n");

        // ��ʾ���з����Ļ�����Ϣ
        FeedbackNode* current = feedback_system->head;
        while (current != NULL) {
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M",
                     localtime(&current->submit_time));
            printf("%-8d %-15s %-20s %-10s\n", current->feedback_id,
                   current->username, time_str,
                   current->status == FEEDBACK_PENDING      ? "������"
                   : current->status == FEEDBACK_PROCESSING ? "������"
                   : current->status == FEEDBACK_RESOLVED   ? "�ѽ��"
                                                            : "�ѹر�");
            current = current->next;
        }

        printf("\n���뷴��ID�鿴��ϸ��Ϣ������0���أ�: ");
        int feedback_id = getValidatedIntegerInput(1000, 9999, 1);

        if (feedback_id == 0) {
            break;
        }

        // ���Ҳ���ʾָ��ID�ķ�����ϸ��Ϣ
        current = feedback_system->head;
        int found = 0;
        while (current != NULL) {
            if (current->feedback_id == feedback_id) {
                found = 1;
                printf("\n----------------------------------------\n");
                printf("������ϸ��Ϣ\n");
                printf("----------------------------------------\n");
                printf("����ID: %d\n", current->feedback_id);
                printf("�û���: %s\n", current->username);
                printf("�ύʱ��: %s", ctime(&current->submit_time));
                outputFeedbackStatus(current);
                printf("��������:\n%s\n", current->content);
                printf("----------------------------------------\n");
                printf(
                    "\n1. ���Ϊ������\n2. ���Ϊ������\n3. ���Ϊ�ѽ��\n4. "
                    "���Ϊ�ѹر�\n0. ������һ��\n��ѡ�����: ");
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
                        printf("\n����״̬�Ѹ���\n");
                    } else {
                        printf("\n״̬����ʧ��\n");
                    }
                    pauseAndClearConsole(1);
                }
                break;
            }
            current = current->next;
        }

        if (!found) {
            printf("δ�ҵ�IDΪ%d�ķ�����Ϣ\n", feedback_id);
        }

        pauseAndClearConsole(0);
    }

    // ������ϵͳ
    FeedbackNode* current = feedback_system->head;
    while (current != NULL) {
        FeedbackNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(feedback_system);
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
    FILE* file = fopen(FEEDBACK_FILE, "wt");
    if (file != NULL) {
        int count = 0;
        fwrite(&count, sizeof(int), 1, file);
        fclose(file);
    } else {
        printf("��շ���ʧ��\n");
    }
}
void outputFeedbackStatus(FeedbackNode* node) {
    const char* status_str;
    FeedbackNode* current = node;
    if (current->status == FEEDBACK_PENDING) {
        status_str = "������";
    } else if (current->status == FEEDBACK_PROCESSING) {
        status_str = "������";
    } else if (current->status == FEEDBACK_RESOLVED) {
        status_str = "�ѽ��";
    } else {
        status_str = "�ѹر�";
    }
    printf("״̬: %s\n", status_str);
}