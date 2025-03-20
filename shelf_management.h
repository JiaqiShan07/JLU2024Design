#ifndef SHELF_MANAGEMENT_H
#define SHELF_MANAGEMENT_H
#include "all_h_files.h"
#define SHELF_COUNT 5      // 货架总数
#define LAYER_COUNT 2      // 每个货架的层数
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
ShelfSystem* initShelfSystem();
int updateShelfStatus(ShelfSystem* shelf_system, PackageSystem* package_system);
void displayShelfStatus(ShelfSystem* system);
void clearShelfSystem(ShelfSystem* system);
int checkShelfCapacityWarning(ShelfSystem* system);
void displayShelfUsage(ShelfSystem* shelf_system,
                       PackageSystem* package_system);

#endif  // SHELF_MANAGEMENT_H
