#include "all_h_files.h"

// ����һ���յ��ַ�����
CharNode* createCharList() {
    return NULL;
}

// ������ĩβ���һ���ַ�
void appendChar(CharNode** head, char c) {
    CharNode* newNode = (CharNode*)malloc(sizeof(CharNode));
    if (newNode == NULL) {
        printf("�ڴ����ʧ��\n");
        exit(1);
    }

    newNode->data = c;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    CharNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

// ���ַ�����ת��Ϊ�ַ���
char* charListToString(CharNode* head) {
    // ����������
    size_t length = 0;
    CharNode* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }

    // �����ڴ�
    char* str = (char*)malloc(length + 1);
    if (str == NULL) {
        printf("�ڴ����ʧ��\n");
        exit(1);
    }

    // �����ַ�
    current = head;
    size_t i = 0;
    while (current != NULL) {
        str[i++] = current->data;
        current = current->next;
    }
    str[i] = '\0';

    return str;
}

// �ͷ��ַ�����
void freeCharList(CharNode* head) {
    CharNode* current = head;
    while (current != NULL) {
        CharNode* next = current->next;
        free(current);
        current = next;
    }
}
int getValidatedIntegerInput(int min, int max, int zero_allowed) {
    int value;
    char* endptr;
    while (1) {
        // �����ַ�����
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // ���ַ���ȡ����
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // ����Ƿ�Ϊ������
        if (isEmpty) {
            printf("������Ч������Ϊ�ա�������һ���� %d �� %d ֮�������: ",
                   min, max);
            freeCharList(inputList);
            continue;
        }

        // ������ת��Ϊ�ַ���
        char* input = charListToString(inputList);

        // ����ת��Ϊ����
        value = strtol(input, &endptr, 10);

        // ����Ƿ�ת���ɹ���û�ж����ַ�
        if ((*endptr == '\0') &&
            ((value >= min && value <= max) || (zero_allowed && value == 0))) {
            freeCharList(inputList);
            free(input);
            return value;
        }

        printf("������Ч��������һ���� %d �� %d ֮�������: ", min, max);
        freeCharList(inputList);
        free(input);
    }
}
float getValidatedFloatInput(float min, float max) {
    float value;
    char* endptr;
    while (1) {
        // �����ַ�����
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // ���ַ���ȡ����
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // ����Ƿ�Ϊ������
        if (isEmpty) {
            printf(
                "������Ч������Ϊ�ա�������һ���� %.2f �� %.2f "
                "֮�������: ",
                min, max);
            freeCharList(inputList);
            continue;
        }

        // ������ת��Ϊ�ַ���
        char* input = charListToString(inputList);

        // ����ת��Ϊ������
        value = strtof(input, &endptr);

        // ����Ƿ�ת���ɹ���û�ж����ַ�
        if (*endptr == '\0' && value >= min && value <= max) {
            freeCharList(inputList);
            free(input);
            return value;
        }

        printf("������Ч��������һ���� %.2f �� %.2f ֮�������: ", min, max);
        freeCharList(inputList);
        free(input);
    }
}
char getValidatedCharInput(const char* valid_chars) {
    char result;
    while (1) {
        // �����ַ�����
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // ���ַ���ȡ����
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // ����Ƿ�Ϊ������
        if (isEmpty) {
            printf("������Ч������Ϊ�ա������������ַ�֮һ: %s\n", valid_chars);
            freeCharList(inputList);
            continue;
        }

        // ������ת��Ϊ�ַ���
        char* input = charListToString(inputList);

        // ��������Ƿ�ֻ��һ���ַ�
        if (strlen(input) == 1) {
            result = input[0];
            // ����ַ��Ƿ�����Ч�ַ��б���
            if (strchr(valid_chars, result) != NULL) {
                freeCharList(inputList);
                free(input);
                return result;
            }
        }

        printf("������Ч�������������ַ�֮һ: %s\n", valid_chars);
        freeCharList(inputList);
        free(input);
    }
}
void getValidatedStringInput(char* buffer, size_t max_length) {
    while (1) {
        // �����ַ�����
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;
        int charCount = 0;

        // ���ַ���ȡ����
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
            charCount++;
        }

        // ����Ƿ�Ϊ������
        if (isEmpty) {
            printf("������Ч������Ϊ�ա����������룺");
            freeCharList(inputList);
            continue;
        }

        // ������ת��Ϊ�ַ���
        char* input = charListToString(inputList);

        // ������볤���Ƿ񳬹��������
        if (charCount >= max_length) {
            printf("���볬����󳤶����ƣ�%zu���ַ��������������룺",
                   max_length - 1);
            freeCharList(inputList);
            free(input);
            continue;
        }

        // ���Ƶ����������
        strcpy(buffer, input);

        freeCharList(inputList);
        free(input);
        break;
    }
}

void getValidatedNumAndLetterInput(char* buffer,
                                   size_t min_length,
                                   size_t max_length) {
    int valid;
    do {
        valid = 1;

        // �����ַ�����
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // ���ַ���ȡ����
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // ����Ƿ�Ϊ������
        if (isEmpty) {
            printf("������Ч������Ϊ�ա����������룺");
            freeCharList(inputList);
            valid = 0;
            continue;
        }

        // ������ת��Ϊ�ַ���
        char* input = charListToString(inputList);
        size_t length = strlen(input);

        // ��鳤���Ƿ����Ҫ��
        if (length < min_length || length > max_length) {
            printf("���볤�Ȳ�����Ҫ�������ԡ������� %zu �� %zu ֮��\n",
                   min_length, max_length);
            printf("���������룺");
            freeCharList(inputList);
            free(input);
            valid = 0;
            continue;
        }

        // ����Ƿ�ֻ������ĸ������
        for (size_t i = 0; i < length; i++) {
            if (!isalnum(input[i])) {
                printf("��������Ƿ��ַ�(��֧����ĸ�����֣�������������:");
                valid = 0;
                break;
            }
        }

        // ���������Ч�����Ƶ����������
        if (valid) {
            strcpy(buffer, input);
        }

        freeCharList(inputList);
        free(input);
    } while (!valid);
}