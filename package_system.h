#pragma once
#include "all_h_files.h"
#include "user_system.h"
#define STRANDED_TIME 1800  // 3�� = 3 * 24 * 60 * 60 = 259200��
#define ONE_DAY 600         // 1�� = 24 * 60 * 60 = 86400��
// ���ļ��м��ذ�����Ϣ��ϵͳ
int loadPackagesFromFile(PackageSystem* system, const char* filename);
// ��ϵͳ�еİ�����Ϣ���浽�ļ�
int savePackagesToFile(PackageSystem* system, const char* filename);
// ��ʼ����������ϵͳ
PackageSystem* initPackageSystem();
// ��ϵͳ������µİ���
int addPackage(PackageSystem* system,
               const char* username,
               const char* description,
               PackageSize size,
               float weight_kg,
               PackageUrgency urgency,
               PackageType type,
               PackageStorage storage,
               PackageStatus status);
// �����û�����ѯ������Ϣ
void queryPackagesByUsername(PackageSystem* system, const char* username);
// �������ȡ������
int pickupPackage(PackageSystem* system, int package_id);
// ��ʾ��������ϸ��Ϣ
void displayPackageDetails(PackageNode* current);
// ���ϵͳ�е����а�����Ϣ
void clearAllPackages(PackageSystem* system);
// ������״̬ת��Ϊ��Ӧ���ַ�������
char* packageSatatusToString(PackageStatus status);
// �����µİ���ID
int generatePackageId(PackageSystem* system);
// ������Ӱ������û�����
void handleAddPackage(PackageSystem* system, UserSystem* user_system);
// �����ѯ�������û�����
void handleQueryPackages(PackageSystem* system, UserSystem* user_system);
// ����ȡ�����û�����
void handlePickupPackage(PackageSystem* system, UserSystem* user_system);
// ����������Ͳ���
void handleDeliverPackage(PackageSystem* system);
// �������ϵͳ���ݵĲ���
int handleClearSystemData(PackageSystem* system, UserSystem* user_system);
// �������쳣�����Ĳ���
void handleMarkAbnormalPackage(PackageSystem* system, UserSystem* user_system);
// ������հ����Ĳ���
void handleRejectPackage(PackageSystem* system, UserSystem* user_system);
// ���������������û���������
void handleStrandedPackages(PackageSystem* system, UserSystem* user_system);
// �������������ĺ��Ĺ��ܺ���
void processStrandedPackages(PackageSystem* system, int days, int package_id);
// Ϊ�°������ɴ洢λ��
int generatePackageLocation(PackageSystem* system,
                            PackageNode* node,
                            int* shelf_number,
                            int* layer_number);
// �������˴�ȡ�������û�����
void handlePickupPackageByOther(PackageSystem* system, UserSystem* user_system);
// ִ�����˴�ȡ�����Ĳ���
void pickupPackageByOther(PackageSystem* system,
                          int package_id,
                          UserNode* current);
// ������Ч�������Ľ�������
void handleCleanInvalidPackageNode(PackageSystem* system);
// ������Ч�������ĺ��Ĺ��ܺ���
void cleanInvalidPackageNode(PackageSystem* system);