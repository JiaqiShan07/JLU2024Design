#include "all_h_files.h"
void pauseAndClearConsole(int getchar_num) {
    if (getchar_num != 0) {
        printf("��Enter������...");
        for (int i = 0; i < getchar_num; i++) {
            getchar();
        }
    }
    system("cls");
}
float calculateDeliveryFee(float weight_kg,
                           int size_choice,
                           int urgency_choice,
                           int type_choice,
                           int storage_choice,
                           UserType user_type) {
    float delivery_fee = weight_kg * 0.8f;

    // ���ݰ�����С��������
    switch (size_choice) {
        case 1:  // �м�
            delivery_fee *= 1.5f;
            break;
        case 2:  // ���
            delivery_fee *= 2.0f;
            break;
        default:  // С�����������շ�
            break;
    }

    // ���ݽ����̶ȵ�������
    if (urgency_choice == 1) {  // �Ӽ�����
        delivery_fee *= 1.5f;
    }

    // ���ݰ������͵�������
    if (type_choice == 1 || type_choice == 2) {  // ������Ʒ��������Ʒ
        delivery_fee *= 1.5f;
    }

    // ���ݴ洢Ҫ���������
    if (storage_choice == 1) {  // ��ط���
        delivery_fee *= 1.5f;
    }

    // Ӧ���û������ۿ�
    switch (user_type) {
        case USER_STUDENT:
            delivery_fee *= 0.8f;  // ѧ������
            break;
        case USER_TEACHER:
            delivery_fee *= 0.7f;  // ��ʦ����
            break;
        case USER_VIP:
            delivery_fee *= 0.6f;  // VIP�û�����
            break;
        default:  // ��ͨ�û��͹���Ա���ۿ�
            break;
    }

    return delivery_fee;
}
// ����һ�����������������г��û��б��Ҳ�ѯָ���û�
// �����ѯ����ָ�����û��᷵������û���㣬���򷵻�NULL
// �Ὣ�û�������û����������ļ��п��Բ�ѯ���ģ���ֵ��target_username
UserNode* getSpecificUser(UserSystem*user_system,char target_username[]) {
    
        printf("\n��ѡ�û��б�:\n");
        printf("----------------------------------------\n");
        UserNode* user = user_system->head;
        while (user != NULL) {
            printf("%s\n", user->username);
            user = user->next;
        }
        printf("----------------------------------------\n");
        printf("������Ҫ���в������û���: ");
        getValidatedStringInput(target_username, MAX_USERNAME_LENGTH);

        // ��֤�û����Ƿ����
        int user_found = 0;
        user = user_system->head;
        while (user != NULL) {
            if (strcmp(user->username, target_username) == 0) {
                user_found = 1;
                return user;
                break;
            }
            user = user->next;
        }
        if (!user_found) {
            printf("----------------------------------------\n");
            printf("δ�ҵ����û�\n");
            printf("----------------------------------------\n");
            return NULL;
        }
    
    return NULL;
}

char* generateInviteCode() {
    char* invite_code = (char*)malloc(6 * sizeof(char));
    if (invite_code == NULL) {
        return NULL;
    }

    // ʹ�õ�ǰʱ��ͽ���ID��Ϊ���������������
    srand((unsigned int)(time(NULL)));

    // ������λ�����д��ĸ (A-Z: 65-90)
    invite_code[0] = (char)(65 + rand() % 26);

    // ���ɺ���λ�������
    for (int i = 1; i < 5; i++) {
        invite_code[i] = (char)(48 + rand() % 10);
    }

    // ����ַ���������
    invite_code[5] = '\0';
    return invite_code;
}