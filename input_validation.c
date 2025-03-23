#include "all_h_files.h"

// 创建一个空的字符链表
CharNode* createCharList() {
    return NULL;
}

// 向链表末尾添加一个字符
void appendChar(CharNode** head, char c) {
    CharNode* newNode = (CharNode*)malloc(sizeof(CharNode));
    if (newNode == NULL) {
        printf("内存分配失败\n");
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

// 将字符链表转换为字符串
char* charListToString(CharNode* head) {
    // 计算链表长度
    size_t length = 0;
    CharNode* current = head;
    while (current != NULL) {
        length++;
        current = current->next;
    }

    // 分配内存
    char* str = (char*)malloc(length + 1);
    if (str == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }

    // 复制字符
    current = head;
    size_t i = 0;
    while (current != NULL) {
        str[i++] = current->data;
        current = current->next;
    }
    str[i] = '\0';

    return str;
}

// 释放字符链表
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
        // 创建字符链表
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // 逐字符读取输入
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // 检查是否为空输入
        if (isEmpty) {
            printf("输入无效，不能为空。请输入一个在 %d 到 %d 之间的整数: ",
                   min, max);
            freeCharList(inputList);
            continue;
        }

        // 将链表转换为字符串
        char* input = charListToString(inputList);

        // 尝试转换为整数
        value = strtol(input, &endptr, 10);

        // 检查是否转换成功且没有多余字符
        if ((*endptr == '\0') &&
            ((value >= min && value <= max) || (zero_allowed && value == 0))) {
            freeCharList(inputList);
            free(input);
            return value;
        }

        printf("输入无效，请输入一个在 %d 到 %d 之间的整数: ", min, max);
        freeCharList(inputList);
        free(input);
    }
}
float getValidatedFloatInput(float min, float max) {
    float value;
    char* endptr;
    while (1) {
        // 创建字符链表
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // 逐字符读取输入
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // 检查是否为空输入
        if (isEmpty) {
            printf(
                "输入无效，不能为空。请输入一个在 %.2f 到 %.2f "
                "之间的数字: ",
                min, max);
            freeCharList(inputList);
            continue;
        }

        // 将链表转换为字符串
        char* input = charListToString(inputList);

        // 尝试转换为浮点数
        value = strtof(input, &endptr);

        // 检查是否转换成功且没有多余字符
        if (*endptr == '\0' && value >= min && value <= max) {
            freeCharList(inputList);
            free(input);
            return value;
        }

        printf("输入无效，请输入一个在 %.2f 到 %.2f 之间的数字: ", min, max);
        freeCharList(inputList);
        free(input);
    }
}
char getValidatedCharInput(const char* valid_chars) {
    char result;
    while (1) {
        // 创建字符链表
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // 逐字符读取输入
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // 检查是否为空输入
        if (isEmpty) {
            printf("输入无效，不能为空。请输入以下字符之一: %s\n", valid_chars);
            freeCharList(inputList);
            continue;
        }

        // 将链表转换为字符串
        char* input = charListToString(inputList);

        // 检查输入是否只有一个字符
        if (strlen(input) == 1) {
            result = input[0];
            // 检查字符是否在有效字符列表中
            if (strchr(valid_chars, result) != NULL) {
                freeCharList(inputList);
                free(input);
                return result;
            }
        }

        printf("输入无效，请输入以下字符之一: %s\n", valid_chars);
        freeCharList(inputList);
        free(input);
    }
}
void getValidatedStringInput(char* buffer, size_t max_length) {
    while (1) {
        // 创建字符链表
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;
        int charCount = 0;

        // 逐字符读取输入
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
            charCount++;
        }

        // 检查是否为空输入
        if (isEmpty) {
            printf("输入无效，不能为空。请重新输入：");
            freeCharList(inputList);
            continue;
        }

        // 将链表转换为字符串
        char* input = charListToString(inputList);

        // 检查输入长度是否超过最大限制
        if (charCount >= max_length) {
            printf("输入超出最大长度限制（%zu个字符），请重新输入：",
                   max_length - 1);
            freeCharList(inputList);
            free(input);
            continue;
        }

        // 复制到输出缓冲区
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

        // 创建字符链表
        CharNode* inputList = createCharList();
        char c;
        int isEmpty = 1;

        // 逐字符读取输入
        while ((c = getchar()) != EOF && c != '\n') {
            appendChar(&inputList, c);
            isEmpty = 0;
        }

        // 检查是否为空输入
        if (isEmpty) {
            printf("输入无效，不能为空。请重新输入：");
            freeCharList(inputList);
            valid = 0;
            continue;
        }

        // 将链表转换为字符串
        char* input = charListToString(inputList);
        size_t length = strlen(input);

        // 检查长度是否符合要求
        if (length < min_length || length > max_length) {
            printf("输入长度不符合要求，请重试。长度在 %zu 到 %zu 之间\n",
                   min_length, max_length);
            printf("请重新输入：");
            freeCharList(inputList);
            free(input);
            valid = 0;
            continue;
        }

        // 检查是否只包含字母和数字
        for (size_t i = 0; i < length; i++) {
            if (!isalnum(input[i])) {
                printf("输入包含非法字符(仅支持字母和数字），请重新输入:");
                valid = 0;
                break;
            }
        }

        // 如果输入有效，复制到输出缓冲区
        if (valid) {
            strcpy(buffer, input);
        }

        freeCharList(inputList);
        free(input);
    } while (!valid);
}