#ifndef USER_TYPES_H
#define USER_TYPES_H
#include "all_h_files.h"
#define USER_FILE "user.txt"
typedef enum {
    USER_STUDENT,  // ѧ���û���20%�ۿ�
    USER_TEACHER,  // ��ʦ�û���30%�ۿ�
    USER_VIP,      // VIP�û���40%�ۿ�
    USER_NORMAL,   // ��ͨ�û������ۿ�
    USER_ADMIN,    // ����Ա�û���ϵͳ����Ȩ�ޣ���ִ�����в���
    USER_COURIER   // ���Ա�û������������Ա��ͬ��Ȩ��
} UserType;
static const char* USER_TYPE_NAMES[] = {"ѧ��",     "��ʦ",   "VIP�û�",
                                        "��ͨ�û�", "����Ա", "���Ա"};
#endif  // USER_TYPES_H