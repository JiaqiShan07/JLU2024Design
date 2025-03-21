#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include "all_h_files.h"
// ��ȡ����֤�������룬ȷ��������ָ����Χ�ڣ���ѡ�Ƿ�������ֵ
int getValidatedIntegerInput(int min, int max, int zero_allowed);
// ��ȡ����֤���������룬ȷ��������ָ����Χ��
float getValidatedFloatInput(float min, float max);
// ��ȡ����֤�ַ����룬ȷ�������ַ���ָ������Ч�ַ�����
char getValidatedCharInput(const char* valid_chars);
// ��ȡ����֤�ַ������룬ȷ�����벻������󳤶�����
void getValidatedStringInput(char* buffer, size_t max_length);
#endif