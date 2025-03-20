#ifndef PACKAGE_TYPES_H
#define PACKAGE_TYPES_H
#include "all_h_files.h"
#define MAX_PACKAGES 1000          // 系统支持的最大包裹数
#define MAX_NAME_LENGTH 50         // 名称的最大长度
#define MAX_USERNAME_LENGTH 50     // 用户名的最大长度
#define MAX_PICKUP_CODE_LENGTH 20  // 取件码的最大长度
#define MAX_ADDRESS_LENGTH 1000    // 地址的最大长度
#define PACKAGE_FILE "packages.txt"
typedef enum {
    SIZE_SMALL,   // 小型，基础运费
    SIZE_MEDIUM,  // 中型，运费 = 基础运费 * 1.5
    SIZE_LARGE    // 大型，运费 = 基础运费 * 2.0
} PackageSize;
typedef enum {
    URGENCY_NORMAL,  // 普通，基础运费
    URGENCY_URGENT   // 加急，运费 = 基础运费 * 1.5
} PackageUrgency;
typedef enum {
    TYPE_NORMAL,    // 普通物品，基础运费
    TYPE_VALUABLE,  // 贵重物品，需要特殊保管，运费 = 基础运费 * 1.5
    TYPE_FRAGILE    // 易碎物品，需要小心搬运，运费 = 基础运费 * 1.5
} PackageType;
typedef enum {
    STORAGE_NORMAL,  // 常温存储，基础运费
    STORAGE_COLD     // 冷藏存储，运费 = 基础运费 * 1.5
} PackageStorage;
typedef enum {
    PENDING_PICKUP,    // 待取件
    PICKED_UP,         // 已取件
    PENDING_DELIVERY,  // 待派送
    DELIVERED,         // 已派送
    REJECTED,          // 已拒收
    ABNORMAL           // 异常
} PackageStatus;
typedef struct PackageNode {
    int package_id;                            // 包裹唯一标识符
    char username[MAX_USERNAME_LENGTH];        // 发件人用户名
    char recipient[MAX_USERNAME_LENGTH];       // 收件人姓名
    char recipientAdress[MAX_ADDRESS_LENGTH];  // 收件人地址
    char description[MAX_NAME_LENGTH];         // 包裹描述
    PackageStatus status;                      // 包裹状态
    PackageSize size;                          // 包裹大小
    float weight_kg;                           // 包裹重量(kg)
    float delivery_fee;                        // 运送费用
    PackageUrgency urgency;                    // 紧急程度
    PackageType type;                          // 包裹类型
    PackageStorage storage;                    // 存储要求
    char pickup_code[MAX_PICKUP_CODE_LENGTH];  // 取件码
    int shelf_number;                          // 存储货架号
    int layer_number;                          // 货架层数
    time_t store_time;                         // 存储时间戳
    time_t pickup_time;                        // 取件时间戳
    time_t sent_time;                          // 派送时间戳
    struct PackageNode* next;                  // 指向下一个包裹节点的指针
} PackageNode;
typedef struct {
    PackageNode* head;  // 包裹链表的头指针
    int package_count;  // 系统中的包裹数量
} PackageSystem;
#endif