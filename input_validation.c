#include "all_h_files.h"
#define MAX_CHECK_SIZE 100000
int getValidatedIntegerInput(int min, int max, int zero_allowed) {
    int value;
    char input[MAX_CHECK_SIZE];
    char* endptr;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // ����Ƿ�Ϊ�����루ֻ�лس���
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf("������Ч������Ϊ�ա�������һ���� %d �� %d ֮�������: ",
                       min, max);
                continue;
            }
            // �Ƴ����з�
            input[strcspn(input, "\n")] = 0;
            // ����ת��Ϊ����
            value = strtol(input, &endptr, 10);
            // ����Ƿ�ת���ɹ���û�ж����ַ�
            if ((*endptr == '\0') && ((value >= min && value <= max) ||
                                      (zero_allowed && value == 0))) {
                return value;
            }
        }
        printf("������Ч��������һ���� %d �� %d ֮�������: ", min, max);
    }
}
float getValidatedFloatInput(float min, float max) {
    float value;
    char input[MAX_CHECK_SIZE];
    char* endptr;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // ����Ƿ�Ϊ�����루ֻ�лس���
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf(
                    "������Ч������Ϊ�ա�������һ���� %.2f �� %.2f "
                    "֮�������: ",
                    min, max);
                continue;
            }
            // �Ƴ����з�
            input[strcspn(input, "\n")] = 0;
            // ����ת��Ϊ������
            value = strtof(input, &endptr);
            // ����Ƿ�ת���ɹ���û�ж����ַ�
            if (*endptr == '\0' && value >= min && value <= max) {
                return value;
            }
        }
        printf("������Ч��������һ���� %.2f �� %.2f ֮�������: ", min, max);
    }
}
char getValidatedCharInput(const char* valid_chars) {
    char input[MAX_CHECK_SIZE];
    char result;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // ����Ƿ�Ϊ�����루ֻ�лس���
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf("������Ч������Ϊ�ա������������ַ�֮һ: %s\n",
                       valid_chars);
                continue;
            }
            // �Ƴ����з�
            input[strcspn(input, "\n")] = 0;
            // ��������Ƿ�ֻ��һ���ַ�
            if (strlen(input) == 1) {
                result = input[0];
                // ����ַ��Ƿ�����Ч�ַ��б���
                if (strchr(valid_chars, result) != NULL) {
                    return result;
                }
            }
        }
        printf("������Ч�������������ַ�֮һ: %s\n", valid_chars);
    }
}
void getValidatedStringInput(char* buffer, size_t max_length) {
    char* newline;
    int input_too_long = 0;
    while (1) {
        if (fgets(buffer, max_length, stdin) != NULL) {
            // ����Ƿ�Ϊ�����루ֻ�лس���
            if (buffer[0] == '\n' ||
                (strlen(buffer) == 1 && buffer[0] == '\r')) {
                printf("������Ч������Ϊ�ա����������룺");
                continue;
            }
            // ���Ҳ��Ƴ����з�
            newline = strchr(buffer, '\n');
            if (newline != NULL) {
                *newline = '\0';
                break;  // ��Ч���룬�˳�ѭ��
            } else {
                // ������볬����󳤶ȣ�������뻺����
                while (getchar() != '\n')
                    ;
                input_too_long = 1;
            }
        }
        // ������볬����󳤶ȣ���ʾ�û�������ѭ��
        if (input_too_long) {
            printf("���볬����󳤶����ƣ�%zu���ַ��������������룺",
                   max_length - 1);
            input_too_long = 0;  // ���ñ�־
        }
    }
}
void getValidatedNumAndLetterInput(char* buffer,
                                   size_t min_length,
                                   size_t max_length) {
    int valid;
    do {
        valid = 1;
        fgets(buffer, max_length + 1, stdin);
        // ȥ�����з�
        buffer[strcspn(buffer, "\n")] = '\0';
        size_t length = strlen(buffer);
        if (length < min_length || length > max_length) {
            printf("���볤�Ȳ�����Ҫ�������ԡ������� %zu �� %zu ֮��\n",
                   min_length, max_length);
            printf("���������룺");
            valid = 0;
            continue;
        }
        for (size_t i = 0; i < length; i++) {
            if (!isalnum(buffer[i])) {
                printf("��������Ƿ��ַ�(��֧����ĸ�����֣�������������:");
                valid = 0;
                break;
            }
        }
    } while (!valid);
}
