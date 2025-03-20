#ifndef USER_TYPES_H
#define USER_TYPES_H
#include "all_h_files.h"
#define USER_FILE "user.txt"
typedef enum {
    USER_STUDENT,  // 学生用户，20%折扣
    USER_TEACHER,  // 教师用户，30%折扣
    USER_VIP,      // VIP用户，40%折扣
    USER_NORMAL,   // 普通用户，无折扣
    USER_ADMIN,    // 管理员用户，系统管理权限，可执行所有操作
    USER_COURIER   // 快递员用户，具有与管理员相同的权限
} UserType;
static const char* USER_TYPE_NAMES[] = {"学生",     "教师",   "VIP用户",
                                        "普通用户", "管理员", "快递员"};
#endif  // USER_TYPES_H