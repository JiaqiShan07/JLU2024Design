#ifndef PACKAGE_TYPES_H
#define PACKAGE_TYPES_H
#include "all_h_files.h"
#define MAX_PACKAGES 1000          // ϵͳ֧�ֵ���������
#define MAX_NAME_LENGTH 50         // ���Ƶ���󳤶�
#define MAX_USERNAME_LENGTH 50     // �û�������󳤶�
#define MAX_PICKUP_CODE_LENGTH 20  // ȡ�������󳤶�
#define MAX_ADDRESS_LENGTH 1000    // ��ַ����󳤶�
#define PACKAGE_FILE "packages.txt"
typedef enum {
    SIZE_SMALL,   // С�ͣ������˷�
    SIZE_MEDIUM,  // ���ͣ��˷� = �����˷� * 1.5
    SIZE_LARGE    // ���ͣ��˷� = �����˷� * 2.0
} PackageSize;
typedef enum {
    URGENCY_NORMAL,  // ��ͨ�������˷�
    URGENCY_URGENT   // �Ӽ����˷� = �����˷� * 1.5
} PackageUrgency;
typedef enum {
    TYPE_NORMAL,    // ��ͨ��Ʒ�������˷�
    TYPE_VALUABLE,  // ������Ʒ����Ҫ���Ᵽ�ܣ��˷� = �����˷� * 1.5
    TYPE_FRAGILE    // ������Ʒ����ҪС�İ��ˣ��˷� = �����˷� * 1.5
} PackageType;
typedef enum {
    STORAGE_NORMAL,  // ���´洢�������˷�
    STORAGE_COLD     // ��ش洢���˷� = �����˷� * 1.5
} PackageStorage;
typedef enum {
    PENDING_PICKUP,    // ��ȡ��
    PICKED_UP,         // ��ȡ��
    PENDING_DELIVERY,  // ������
    DELIVERED,         // ������
    REJECTED,          // �Ѿ���
    ABNORMAL           // �쳣
} PackageStatus;
typedef struct PackageNode {
    int package_id;                            // ����Ψһ��ʶ��
    char username[MAX_USERNAME_LENGTH];        // �������û���
    char recipient[MAX_USERNAME_LENGTH];       // �ռ�������
    char recipientAdress[MAX_ADDRESS_LENGTH];  // �ռ��˵�ַ
    char description[MAX_NAME_LENGTH];         // ��������
    PackageStatus status;                      // ����״̬
    PackageSize size;                          // ������С
    float weight_kg;                           // ��������(kg)
    float delivery_fee;                        // ���ͷ���
    PackageUrgency urgency;                    // �����̶�
    PackageType type;                          // ��������
    PackageStorage storage;                    // �洢Ҫ��
    char pickup_code[MAX_PICKUP_CODE_LENGTH];  // ȡ����
    int shelf_number;                          // �洢���ܺ�
    int layer_number;                          // ���ܲ���
    time_t store_time;                         // �洢ʱ���
    time_t pickup_time;                        // ȡ��ʱ���
    time_t sent_time;                          // ����ʱ���
    struct PackageNode* next;                  // ָ����һ�������ڵ��ָ��
} PackageNode;
typedef struct {
    PackageNode* head;  // ���������ͷָ��
    int package_count;  // ϵͳ�еİ�������
} PackageSystem;
#endif