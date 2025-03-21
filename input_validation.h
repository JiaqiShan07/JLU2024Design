#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include "all_h_files.h"
// 获取并验证整数输入，确保输入在指定范围内，可选是否允许零值
int getValidatedIntegerInput(int min, int max, int zero_allowed);
// 获取并验证浮点数输入，确保输入在指定范围内
float getValidatedFloatInput(float min, float max);
// 获取并验证字符输入，确保输入字符在指定的有效字符集中
char getValidatedCharInput(const char* valid_chars);
// 获取并验证字符串输入，确保输入不超过最大长度限制
void getValidatedStringInput(char* buffer, size_t max_length);
#endif