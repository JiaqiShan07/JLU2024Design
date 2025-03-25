#pragma once
#include "all_h_files.h"
#include "user_system.h"
#define STRANDED_TIME 1800  // 3天 = 3 * 24 * 60 * 60 = 259200秒
#define ONE_DAY 600         // 1天 = 24 * 60 * 60 = 86400秒
// 从文件中加载包裹信息到系统
int loadPackagesFromFile(PackageSystem* system, const char* filename);
// 将系统中的包裹信息保存到文件
int savePackagesToFile(PackageSystem* system, const char* filename);
// 初始化包裹管理系统
PackageSystem* initPackageSystem();
// 向系统中添加新的包裹
int addPackage(PackageSystem* system,
               const char* username,
               const char* description,
               PackageSize size,
               float weight_kg,
               PackageUrgency urgency,
               PackageType type,
               PackageStorage storage,
               PackageStatus status);
// 根据用户名查询包裹信息
void queryPackagesByUsername(PackageSystem* system, const char* username);
// 处理包裹取件操作
int pickupPackage(PackageSystem* system, int package_id);
// 显示包裹的详细信息
void displayPackageDetails(PackageNode* current);
// 清空系统中的所有包裹信息
void clearAllPackages(PackageSystem* system);
// 将包裹状态转换为对应的字符串描述
char* packageSatatusToString(PackageStatus status);
// 生成新的包裹ID
int generatePackageId(PackageSystem* system);
// 处理添加包裹的用户交互
void handleAddPackage(PackageSystem* system, UserSystem* user_system);
// 处理查询包裹的用户交互
void handleQueryPackages(PackageSystem* system, UserSystem* user_system);
// 处理取件的用户交互
void handlePickupPackage(PackageSystem* system, UserSystem* user_system);
// 处理包裹派送操作
void handleDeliverPackage(PackageSystem* system);
// 处理清空系统数据的操作
int handleClearSystemData(PackageSystem* system, UserSystem* user_system);
// 处理标记异常包裹的操作
void handleMarkAbnormalPackage(PackageSystem* system, UserSystem* user_system);
// 处理拒收包裹的操作
void handleRejectPackage(PackageSystem* system, UserSystem* user_system);
// 处理滞留包裹的用户交互函数
void handleStrandedPackages(PackageSystem* system, UserSystem* user_system);
// 处理滞留包裹的核心功能函数
void processStrandedPackages(PackageSystem* system, int days, int package_id);
// 为新包裹生成存储位置
int generatePackageLocation(PackageSystem* system,
                            PackageNode* node,
                            int* shelf_number,
                            int* layer_number);
// 处理他人代取包裹的用户交互
void handlePickupPackageByOther(PackageSystem* system, UserSystem* user_system);
// 执行他人代取包裹的操作
void pickupPackageByOther(PackageSystem* system,
                          int package_id,
                          UserNode* current);
// 清理无效包裹结点的交互函数
void handleCleanInvalidPackageNode(PackageSystem* system);
// 清理无效包裹结点的核心功能函数
void cleanInvalidPackageNode(PackageSystem* system);