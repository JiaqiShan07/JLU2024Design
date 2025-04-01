#include "predict_system.h"
#include "all_h_files.h"
#include "feedback_assistant.h"
predict_system* initialpredictSystem() {
    // Ϊϵͳ�ṹ������ڴ�
    predict_system* system = (predict_system*)malloc(sizeof(predict_system));
    if (system == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return NULL;
    }
    // ��ʼ��ϵͳ״̬
    system->weather = SUNNY;           // ��ʼ������
    system->strenthregistration = 20;  // �ٶ�ע����Ϊ20��
    system->strenthVIP = 5;            // �ٶ�һ�������˻��ֵVIP
    system->strenthpackage = 5;        // �ٶ�һ��������İ�����ĿΪ5
    system->strenthwater = 200;        // �ٶ�һ�����ˮΪ250Ԫ
    return system;
}
void registrationpredict(predict_system* system,
                         UserSystem* user_system,
                         FeedbackSystem* feedback) {
    UserNode* prediction = user_system->head;
    if (prediction->friend_count >= 8) {
        system->strenthregistration += 5;
    } else if (prediction->friend_count >= 5 && prediction->friend_count < 8) {
        system->strenthregistration += 3;
    } else if (prediction->friend_count >= 2 && prediction->friend_count < 5) {
        system->strenthregistration += 1;
    } else {
        system->strenthregistration += 0;
    }
    feedback = loadFeedbackFromFile();
    if (feedback->feedback_count >= 20) {
        system->strenthregistration += 5;
    } else if (feedback->feedback_count >= 15 &&
               feedback->feedback_count < 20) {
        system->strenthregistration += 4;
    } else if (feedback->feedback_count >= 10 &&
               feedback->feedback_count < 15) {
        system->strenthregistration += 3;
    } else if (feedback->feedback_count >= 5 && feedback->feedback_count < 10) {
        system->strenthregistration += 2;
    } else {
        system->strenthregistration += 0;
    }
}
void VIPpredict(predict_system* system, UserSystem* user_system) {
    UserNode* prediction = user_system->head;
    while (prediction != NULL) {
        if (prediction->packagepre >= 5) {
            system->strenthVIP++;
        }
        prediction = prediction->next;
    }
}
int packageandwaterpredict(PackageSystem* system,
                           predict_system* predict_System,
                           UserSystem* user_system) {
    int choice = getValidatedIntegerInput(1, 4, 1);
    switch (choice) {
        case 1:
            predict_System->strenthpackage += -3;
            predict_System->strenthwater += 100;
            break;
        case 2:
            predict_System->strenthpackage += 2;
            predict_System->strenthwater += -50;
            break;
        case 3:
            predict_System->strenthpackage += 1;
            predict_System->strenthwater += -30;
            break;
        case 4:
            predict_System->strenthpackage += 3;
            predict_System->strenthwater += -100;
            break;
        case 0:
            return 0;
        default:
            printf("��Ч��ѡ�������ѡ��\n");
            break;
    }
    int totalstrange = 0;  // ���������ܰ�����Ŀ
    PackageNode* current = system->head;
    while (current != NULL) {
        if (current->stranded_time == 2) {
            totalstrange++;
        }
        current = current->next;
    }
    if (totalstrange >= 5) {
        predict_System->strenthpackage += 3;
    } else if (totalstrange >= 3 && totalstrange < 5) {
        predict_System->strenthpackage += 2;
    } else if (totalstrange >= 0 && totalstrange < 3) {
        predict_System->strenthpackage += 1;
    }
    int num = 0;
    if (user_system->user_count >= 50) {
        predict_System->strenthwater += 300;
    } else if (user_system->user_count >= 30 && user_system->user_count < 50) {
        predict_System->strenthwater += 100;
    } else if (user_system->user_count >= 20 && user_system->user_count < 30) {
        predict_System->strenthwater += 50;
    } else if (user_system->user_count >= 10 && user_system->user_count < 20) {
        predict_System->strenthwater += 0;
    } else if (user_system->user_count >= 0 && user_system->user_count < 10) {
        predict_System->strenthwater += -100;
    }
    UserNode* current_user = user_system->head;
    while (current_user != NULL) {
        if (current_user->type == USER_VIP) {
            num++;
        }
        current_user = current_user->next;
    }
    if (num >= 10) {
        predict_System->strenthwater += 100;
    } else if (num >= 8 && num < 10) {
        predict_System->strenthwater += 80;
    } else if (num >= 6 && num < 8) {
        predict_System->strenthwater += 60;
    } else if (num >= 4 && num < 6) {
        predict_System->strenthwater += 40;
    } else if (num >= 2 && num < 4) {
        predict_System->strenthwater += 20;
    } else {
        predict_System->strenthwater += 0;
    }
    return 1;
}
int handlepredictSystem(UserSystem* user_system,
                        PackageSystem* package_system,
                        FeedbackSystem* feedback) {
    predict_system* pre_system = initialpredictSystem();
    registrationpredict(pre_system, user_system, feedback);
    VIPpredict(pre_system, user_system);
    if (!packageandwaterpredict(package_system, pre_system, user_system)) {
        return 0;
    }
    printf("��Ԥ�⣬����Ԥ��ע������ԼΪ%d��\n",
           pre_system->strenthregistration);
    printf("��Ԥ�⣬�����û�Ԥ����ֵ��ΪVIP����ԼΪ%d\n",
           pre_system->strenthVIP);
    printf("��Ԥ�⣬����Ԥ������������ĿԼΪ%d\n", pre_system->strenthpackage);
    printf("��Ԥ�⣬����Ԥ������ˮԼΪ%dԪ\n", pre_system->strenthwater);
    printf("----------------------------------------\n");
   free(pre_system);
    return 0;
}