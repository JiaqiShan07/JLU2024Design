#pragma once
#include "all_h_files.h"
#include "user_system.h"
#define STRANDED_TIME 10  // 3天 = 3 * 24 * 60 * 60 = 259200秒
#define ONE_DAY 3  // 1天 = 24 * 60 * 60 = 86400秒
int loadPackagesFromFile(PackageSystem* system, const char* filename);
int savePackagesToFile(PackageSystem* system, const char* filename);
PackageSystem* initPackageSystem();
int addPackage(PackageSystem* system,
               const char* username,
               const char* description,
               PackageSize size,
               float weight_kg,
               PackageUrgency urgency,
               PackageType type,
               PackageStorage storage,
               PackageStatus status);
void queryPackagesByUsername(PackageSystem* system, const char* username);
int pickupPackage(PackageSystem* system, int package_id);
void displayPackageDetails(PackageNode* current);
void clearAllPackages(PackageSystem* system);
char* packageSatatusToString(PackageStatus status);
int generatePackageId(PackageSystem* system);
void handleAddPackage(PackageSystem* system, UserSystem* user_system);
void handleQueryPackages(PackageSystem* system, UserSystem* user_system);
void handlePickupPackage(PackageSystem* system, UserSystem* user_system);
void handleDeliverPackage(PackageSystem* system);
void handleClearSystemData(PackageSystem* system, UserSystem* user_system);
void handleMarkAbnormalPackage(PackageSystem* system, UserSystem* user_system);
void handleRejectPackage(PackageSystem* system, UserSystem* user_system);
// 处理滞留包裹的用户交互函数
void handleStrandedPackages(PackageSystem* system, UserSystem* user_system);
// 处理滞留包裹的核心功能函数
void processStrandedPackages(PackageSystem* system, int days, int package_id);
// 获取用户列表，并且选择用户
int generatePackageLocation(PackageSystem* system,
                            int* shelf_number,
                            int* layer_number);
void handlePickupPackageByOther(PackageSystem* system, UserSystem* user_system);
void pickupPackageByOther(PackageSystem* system,
                          int package_id,
                          UserNode* current);