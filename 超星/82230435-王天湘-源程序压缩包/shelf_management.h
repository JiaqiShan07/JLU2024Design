#ifndef SHELF_MANAGEMENT_H
#define SHELF_MANAGEMENT_H
#include "all_h_files.h"
#define SHELF_COUNT 5      // ��������
#define LAYER_COUNT 3      // ÿ�����ܵĲ���
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
// ��ʼ�����ܹ���ϵͳ
ShelfSystem* initShelfSystem();
// ���»���״̬��Ϣ
int updateShelfStatus(ShelfSystem* shelf_system, PackageSystem* package_system);
// ��ʾ��ǰ����״̬
void displayShelfStatus(ShelfSystem* system);
// ��ջ���ϵͳ����
void clearShelfSystem(ShelfSystem* system);
// ��������������
int checkShelfCapacityWarning(ShelfSystem* system);
// ��ʾ����ʹ�����
void displayShelfUsage(ShelfSystem* shelf_system,
                       PackageSystem* package_system);
// ��ʾ���а�����Ϣ
void displayAllPackages(PackageSystem* system);
// �������ͳ����Ϣ
void handlePackageStatistics(PackageSystem* system, UserSystem* user_system);
// ��ʾ���ܾ�����Ϣ
void displayWarningMessage(ShelfSystem* shelf_system);
// �����ܺ�ת��Ϊ���ܵ�����
// ���ܵķ���ԭ��
//  ���ݰ������ͺʹ�С������ܺţ������������
//  1-3�Ż��ֱܷ���ô���С�ŷ���ذ���
//  4�Ż���ר�ŷ�����ذ���
//  5�Ż���ר�ŷ��ù��ذ���
char* switchShelfNumToString(int shelf_num);
#endif
