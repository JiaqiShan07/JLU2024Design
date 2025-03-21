#include "all_h_files.h"

ShelfSystem* initShelfSystem() {
    ShelfSystem* system = (ShelfSystem*)malloc(sizeof(ShelfSystem));
    if (system == NULL) {
        return NULL;
    }

    system->head = NULL;
    system->shelf_count = 0;

    // 初始化4个货架
    for (int i = 0; i < SHELF_COUNT; i++) {
        ShelfNode* new_node = (ShelfNode*)malloc(sizeof(ShelfNode));
        if (new_node == NULL) {
            clearShelfSystem(system);
            return NULL;
        }

        new_node->shelf_number = i + 1;
        for (int j = 0; j < LAYER_COUNT; j++) {
            new_node->packages_per_layer[j] = 0;
        }

        // 将新节点插入到链表头部
        new_node->next = system->head;
        system->head = new_node;
        system->shelf_count++;
    }

    return system;
}

int updateShelfStatus(ShelfSystem* shelf_system,
                      PackageSystem* package_system) {
    if (shelf_system == NULL || package_system == NULL) {
        printf("货架系统或包裹系统未初始化！\n");
        return 0;
    }

    // 重置所有货架的包裹数量
    ShelfNode* current = shelf_system->head;
    while (current != NULL) {
        for (int i = 0; i < LAYER_COUNT; i++) {
            current->packages_per_layer[i] = 0;
        }
        current = current->next;
    }

    // 遍历包裹链表，统计货架使用情况
    PackageNode* package = package_system->head;
    int updated_count = 0;

    while (package != NULL) {
        // 只统计未寄出和未取出和滞留的包裹
        if (package->status != PICKED_UP && package->status != DELIVERED&&package->status!=STRANDED) {
            // 检查包裹位置信息的有效性
            if (package->shelf_number > 0 &&
                package->shelf_number <= SHELF_COUNT &&
                package->layer_number >= 0 &&
                package->layer_number < LAYER_COUNT) {
                // 查找对应的货架节点
                current = shelf_system->head;
                while (current != NULL) {
                    if (current->shelf_number == package->shelf_number) {
                        current->packages_per_layer[package->layer_number]++;
                        updated_count++;
                        break;
                    }
                    current = current->next;
                }
            } else {
                printf(
                    "警告：包裹ID %d 的位置信息无效（货架号：%d，层号：%d）\n",
                    package->package_id, package->shelf_number,
                    package->layer_number);
            }
        }
        package = package->next;
    }

    printf("已更新 %d 个包裹的位置信息\n", updated_count);
    return 1;
}

void displayShelfStatus(ShelfSystem* system) {
    if (system == NULL) {
        printf("货架系统未初始化！\n");
        return;
    }

    // 计算实际的货架数量
    int total_shelves = system->shelf_count;
    if (total_shelves == 0) {
        printf("系统中没有货架！\n");
        return;
    }

    printf("\n=== 货架使用状态 ===\n");

    // 创建一个标记数组，用于记录已显示的货架号
    int displayed[SHELF_COUNT + 1] = {0};  // 下标0不使用，1-5对应货架号1-5
    ShelfNode* current = system->head;

    // 按照货架号1-5的顺序显示
    for (int shelf_num = 1; shelf_num <= SHELF_COUNT; shelf_num++) {
        // 查找对应货架号的节点
        current = system->head;
        while (current != NULL) {
            if (current->shelf_number == shelf_num) {
                printf("\n货架 #%d:\n", current->shelf_number);
                for (int j = 0; j < LAYER_COUNT; j++) {
                    printf("第 %d 层: %d/%d\n", j + 1,
                           current->packages_per_layer[j], LAYER_CAPACITY);
                }
                displayed[shelf_num] = 1;
                break;
            }
            current = current->next;
        }
    }

    // 检查是否有未显示的货架（用于错误检测）
    for (int i = 1; i <= SHELF_COUNT; i++) {
        if (!displayed[i]) {
            printf("\n警告：未找到货架 #%d\n", i);
        }
    }

    printf("\n货架总数: %d\n", system->shelf_count);
}

int checkShelfCapacityWarning(ShelfSystem* system) {
    if (system == NULL) {
        return 0;
    }

    int warning_count = 0;
    ShelfNode* current = system->head;

    while (current != NULL) {
        for (int i = 0; i < LAYER_COUNT; i++) {
            // 计算剩余容量的百分比
            int remaining = LAYER_CAPACITY - current->packages_per_layer[i];
            float remaining_percentage =
                (float)remaining / LAYER_CAPACITY * 100;

            // 如果剩余容量小于10%，增加预警计数
            if (remaining_percentage < 10.0) {
                warning_count++;
                printf(
                    "警告：货架 #%d "
                    "第 %d 层剩余容量不足10%% (当前剩余: %.1f%%)\n",
                    current->shelf_number, i + 1, remaining_percentage);
            }
        }
        current = current->next;
    }

    return warning_count;
}

void displayShelfUsage(ShelfSystem* shelf_system,
                       PackageSystem* package_system) {
    if (shelf_system == NULL || package_system == NULL) {
        printf("系统未初始化！\n");
        return;
    }

    // Update shelf status
    if (!updateShelfStatus(shelf_system, package_system)) {
        printf("更新货架状态失败！\n");
        return;
    }

    // Display shelf usage status
    displayShelfStatus(shelf_system);

    // Check and display capacity warnings
    printf("\n=== 货架容量预警 ===\n");
    int warning_count = checkShelfCapacityWarning(shelf_system);
    if (warning_count == 0) {
        printf("当前没有货架容量预警。\n");
    } else {
        printf("\n共有 %d 个货架层需要注意容量问题。\n", warning_count);
    }
}

void clearShelfSystem(ShelfSystem* system) {
    if (system == NULL) {
        return;
    }

    ShelfNode* current = system->head;
    while (current != NULL) {
        ShelfNode* temp = current;
        current = current->next;
        free(temp);
    }

    system->head = NULL;
    system->shelf_count = 0;
}