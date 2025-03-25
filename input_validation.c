#include "all_h_files.h"
#define MAX_CHECK_SIZE 100000
int getValidatedIntegerInput(int min, int max, int zero_allowed) {
    int value;
    char input[MAX_CHECK_SIZE];
    char* endptr;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // 检查是否为空输入（只有回车）
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf("输入无效，不能为空。请输入一个在 %d 到 %d 之间的整数: ",
                       min, max);
                continue;
            }
            // 移除换行符
            input[strcspn(input, "\n")] = 0;
            // 尝试转换为整数
            value = strtol(input, &endptr, 10);
            // 检查是否转换成功且没有多余字符
            if ((*endptr == '\0') && ((value >= min && value <= max) ||
                                      (zero_allowed && value == 0))) {
                return value;
            }
        }
        printf("输入无效，请输入一个在 %d 到 %d 之间的整数: ", min, max);
    }
}
float getValidatedFloatInput(float min, float max) {
    float value;
    char input[MAX_CHECK_SIZE];
    char* endptr;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // 检查是否为空输入（只有回车）
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf(
                    "输入无效，不能为空。请输入一个在 %.2f 到 %.2f "
                    "之间的数字: ",
                    min, max);
                continue;
            }
            // 移除换行符
            input[strcspn(input, "\n")] = 0;
            // 尝试转换为浮点数
            value = strtof(input, &endptr);
            // 检查是否转换成功且没有多余字符
            if (*endptr == '\0' && value >= min && value <= max) {
                return value;
            }
        }
        printf("输入无效，请输入一个在 %.2f 到 %.2f 之间的数字: ", min, max);
    }
}
char getValidatedCharInput(const char* valid_chars) {
    char input[MAX_CHECK_SIZE];
    char result;
    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // 检查是否为空输入（只有回车）
            if (input[0] == '\n' || (strlen(input) == 1 && input[0] == '\r')) {
                printf("输入无效，不能为空。请输入以下字符之一: %s\n",
                       valid_chars);
                continue;
            }
            // 移除换行符
            input[strcspn(input, "\n")] = 0;
            // 检查输入是否只有一个字符
            if (strlen(input) == 1) {
                result = input[0];
                // 检查字符是否在有效字符列表中
                if (strchr(valid_chars, result) != NULL) {
                    return result;
                }
            }
        }
        printf("输入无效，请输入以下字符之一: %s\n", valid_chars);
    }
}
void getValidatedStringInput(char* buffer, size_t max_length) {
    char* newline;
    int input_too_long = 0;
    while (1) {
        if (fgets(buffer, max_length, stdin) != NULL) {
            // 检查是否为空输入（只有回车）
            if (buffer[0] == '\n' ||
                (strlen(buffer) == 1 && buffer[0] == '\r')) {
                printf("输入无效，不能为空。请重新输入：");
                continue;
            }
            // 查找并移除换行符
            newline = strchr(buffer, '\n');
            if (newline != NULL) {
                *newline = '\0';
                break;  // 有效输入，退出循环
            } else {
                // 如果输入超过最大长度，清除输入缓冲区
                while (getchar() != '\n')
                    ;
                input_too_long = 1;
            }
        }
        // 如果输入超出最大长度，提示用户并继续循环
        if (input_too_long) {
            printf("输入超出最大长度限制（%zu个字符），请重新输入：",
                   max_length - 1);
            input_too_long = 0;  // 重置标志
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
        // 去掉换行符
        buffer[strcspn(buffer, "\n")] = '\0';
        size_t length = strlen(buffer);
        if (length < min_length || length > max_length) {
            printf("输入长度不符合要求，请重试。长度在 %zu 到 %zu 之间\n",
                   min_length, max_length);
            printf("请重新输入：");
            valid = 0;
            continue;
        }
        for (size_t i = 0; i < length; i++) {
            if (!isalnum(buffer[i])) {
                printf("输入包含非法字符(仅支持字母和数字），请重新输入:");
                valid = 0;
                break;
            }
        }
    } while (!valid);
}
