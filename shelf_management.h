#ifndef SHELF_MANAGEMENT_H
#define SHELF_MANAGEMENT_H
#include "all_h_files.h"
#define SHELF_COUNT 5      // ��������
#define LAYER_COUNT 2      // ÿ�����ܵĲ���
#define LAYER_CAPACITY 10  // ÿ����������
typedef struct ShelfNode {
    int shelf_number;                     // ���ܱ��
    int packages_per_layer[LAYER_COUNT];  // ÿ�㵱ǰ��������
    struct ShelfNode* next;               // ָ����һ�����ܽڵ��ָ��
} ShelfNode;
typedef struct {
    ShelfNode* head;  // ���������ͷָ��
    int shelf_count;  // ��ǰ��������
} ShelfSystem;
ShelfSystem* initShelfSystem();
int updateShelfStatus(ShelfSystem* shelf_system, PackageSystem* package_system);
void displayShelfStatus(ShelfSystem* system);
void clearShelfSystem(ShelfSystem* system);
int checkShelfCapacityWarning(ShelfSystem* system);
void displayShelfUsage(ShelfSystem* shelf_system,
                       PackageSystem* package_system);

#endif  // SHELF_MANAGEMENT_H
