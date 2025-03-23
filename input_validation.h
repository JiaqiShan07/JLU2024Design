#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include "all_h_files.h"

// ��������ڵ�ṹ�壬���ڴ洢�����ַ�
typedef struct CharNode {
    char data;
    struct CharNode* next;
} CharNode;

// �����������
CharNode* createCharList();
void appendChar(CharNode** head, char c);
char* charListToString(CharNode* head);
void freeCharList(CharNode* head);
// ��ȡ����֤�������룬ȷ��������ָ����Χ�ڣ���ѡ�Ƿ�������ֵ
int getValidatedIntegerInput(int min, int max, int zero_allowed);
// ��ȡ����֤���������룬ȷ��������ָ����Χ��
float getValidatedFloatInput(float min, float max);
// ��ȡ����֤�ַ����룬ȷ�������ַ���ָ������Ч�ַ�����
char getValidatedCharInput(const char* valid_chars);
// ��ȡ����֤�ַ������룬ȷ�����벻������󳤶�����
void getValidatedStringInput(char* buffer, size_t max_length);
// ��ȡ����ָ֤����Χ�����ֺ���ĸ���룬ȷ�����볤����ָ����Χ��
void getValidatedNumAndLetterInput(char* buffer,
                                   size_t min_length,
                                   size_t max_length);
#endif