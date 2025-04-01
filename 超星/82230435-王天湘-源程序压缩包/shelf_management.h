#ifndef SHELF_MANAGEMENT_H
#define SHELF_MANAGEMENT_H
#include "all_h_files.h"
#define SHELF_COUNT 5      // 货架总数
#define LAYER_COUNT 3      // 每个货架的层数
#define LAYER_CAPACITY 10  // 每层容量限制
typedef struct ShelfNode {
    int shelf_number;                     // 货架编号
    int packages_per_layer[LAYER_COUNT];  // 每层当前包裹数量
    struct ShelfNode* next;               // 指向下一个货架节点的指针
} ShelfNode;
typedef struct {
    ShelfNode* head;  // 货架链表的头指针
    int shelf_count;  // 当前货架数量
} ShelfSystem;
// 初始化货架管理系统
ShelfSystem* initShelfSystem();
// 更新货架状态信息
int updateShelfStatus(ShelfSystem* shelf_system, PackageSystem* package_system);
// 显示当前货架状态
void displayShelfStatus(ShelfSystem* system);
// 清空货架系统数据
void clearShelfSystem(ShelfSystem* system);
// 检查货架容量警告
int checkShelfCapacityWarning(ShelfSystem* system);
// 显示货架使用情况
void displayShelfUsage(ShelfSystem* shelf_system,
                       PackageSystem* package_system);
// 显示所有包裹信息
void displayAllPackages(PackageSystem* system);
// 处理包裹统计信息
void handlePackageStatistics(PackageSystem* system, UserSystem* user_system);
// 显示货架警告信息
void displayWarningMessage(ShelfSystem* shelf_system);
// 将货架号转换为货架的名字
// 货架的分配原则
//  根据包裹类型和大小分配货架号，层数随机生成
//  1-3号货架分别放置大中小号非冷藏包裹
//  4号货架专门放置冷藏包裹
//  5号货架专门放置贵重包裹
char* switchShelfNumToString(int shelf_num);
#endif
