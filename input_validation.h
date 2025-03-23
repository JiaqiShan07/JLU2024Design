#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include "all_h_files.h"

// 定义链表节点结构体，用于存储输入字符
typedef struct CharNode {
    char data;
    struct CharNode* next;
} CharNode;

// 链表操作函数
CharNode* createCharList();
void appendChar(CharNode** head, char c);
char* charListToString(CharNode* head);
void freeCharList(CharNode* head);
// 获取并验证整数输入，确保输入在指定范围内，可选是否允许零值
int getValidatedIntegerInput(int min, int max, int zero_allowed);
// 获取并验证浮点数输入，确保输入在指定范围内
float getValidatedFloatInput(float min, float max);
// 获取并验证字符输入，确保输入字符在指定的有效字符集中
char getValidatedCharInput(const char* valid_chars);
// 获取并验证字符串输入，确保输入不超过最大长度限制
void getValidatedStringInput(char* buffer, size_t max_length);
// 获取并验证指定范围的数字和字母输入，确保输入长度在指定范围内
void getValidatedNumAndLetterInput(char* buffer,
                                   size_t min_length,
                                   size_t max_length);
#endif