#include "all_h_files.h"

ShelfSystem* initShelfSystem() {
    ShelfSystem* system = (ShelfSystem*)malloc(sizeof(ShelfSystem));
    if (system == NULL) {
        return NULL;
    }

    system->head = NULL;
    system->shelf_count = 0;

    // ��ʼ��4������
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

        // ���½ڵ���뵽����ͷ��
        new_node->next = system->head;
        system->head = new_node;
        system->shelf_count++;
    }

    return system;
}

int updateShelfStatus(ShelfSystem* shelf_system,
                      PackageSystem* package_system) {
    if (shelf_system == NULL || package_system == NULL) {
        printf("����ϵͳ�����ϵͳδ��ʼ����\n");
        return 0;
    }

    // �������л��ܵİ�������
    ShelfNode* current = shelf_system->head;
    while (current != NULL) {
        for (int i = 0; i < LAYER_COUNT; i++) {
            current->packages_per_layer[i] = 0;
        }
        current = current->next;
    }

    // ������������ͳ�ƻ���ʹ�����
    PackageNode* package = package_system->head;
    int updated_count = 0;

    while (package != NULL) {
        // ֻͳ��δ�ĳ���δȡ���������İ���
        if (package->status != PICKED_UP && package->status != DELIVERED&&package->status!=STRANDED) {
            // ������λ����Ϣ����Ч��
            if (package->shelf_number > 0 &&
                package->shelf_number <= SHELF_COUNT &&
                package->layer_number >= 0 &&
                package->layer_number < LAYER_COUNT) {
                // ���Ҷ�Ӧ�Ļ��ܽڵ�
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
                    "���棺����ID %d ��λ����Ϣ��Ч�����ܺţ�%d����ţ�%d��\n",
                    package->package_id, package->shelf_number,
                    package->layer_number);
            }
        }
        package = package->next;
    }

    printf("�Ѹ��� %d ��������λ����Ϣ\n", updated_count);
    return 1;
}

void displayShelfStatus(ShelfSystem* system) {
    if (system == NULL) {
        printf("����ϵͳδ��ʼ����\n");
        return;
    }

    // ����ʵ�ʵĻ�������
    int total_shelves = system->shelf_count;
    if (total_shelves == 0) {
        printf("ϵͳ��û�л��ܣ�\n");
        return;
    }

    printf("\n=== ����ʹ��״̬ ===\n");

    // ����һ��������飬���ڼ�¼����ʾ�Ļ��ܺ�
    int displayed[SHELF_COUNT + 1] = {0};  // �±�0��ʹ�ã�1-5��Ӧ���ܺ�1-5
    ShelfNode* current = system->head;

    // ���ջ��ܺ�1-5��˳����ʾ
    for (int shelf_num = 1; shelf_num <= SHELF_COUNT; shelf_num++) {
        // ���Ҷ�Ӧ���ܺŵĽڵ�
        current = system->head;
        while (current != NULL) {
            if (current->shelf_number == shelf_num) {
                printf("\n���� #%d:\n", current->shelf_number);
                for (int j = 0; j < LAYER_COUNT; j++) {
                    printf("�� %d ��: %d/%d\n", j + 1,
                           current->packages_per_layer[j], LAYER_CAPACITY);
                }
                displayed[shelf_num] = 1;
                break;
            }
            current = current->next;
        }
    }

    // ����Ƿ���δ��ʾ�Ļ��ܣ����ڴ����⣩
    for (int i = 1; i <= SHELF_COUNT; i++) {
        if (!displayed[i]) {
            printf("\n���棺δ�ҵ����� #%d\n", i);
        }
    }

    printf("\n��������: %d\n", system->shelf_count);
}

int checkShelfCapacityWarning(ShelfSystem* system) {
    if (system == NULL) {
        return 0;
    }

    int warning_count = 0;
    ShelfNode* current = system->head;

    while (current != NULL) {
        for (int i = 0; i < LAYER_COUNT; i++) {
            // ����ʣ�������İٷֱ�
            int remaining = LAYER_CAPACITY - current->packages_per_layer[i];
            float remaining_percentage =
                (float)remaining / LAYER_CAPACITY * 100;

            // ���ʣ������С��10%������Ԥ������
            if (remaining_percentage < 10.0) {
                warning_count++;
                printf(
                    "���棺���� #%d "
                    "�� %d ��ʣ����������10%% (��ǰʣ��: %.1f%%)\n",
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
        printf("ϵͳδ��ʼ����\n");
        return;
    }

    // Update shelf status
    if (!updateShelfStatus(shelf_system, package_system)) {
        printf("���»���״̬ʧ�ܣ�\n");
        return;
    }

    // Display shelf usage status
    displayShelfStatus(shelf_system);

    // Check and display capacity warnings
    printf("\n=== ��������Ԥ�� ===\n");
    int warning_count = checkShelfCapacityWarning(shelf_system);
    if (warning_count == 0) {
        printf("��ǰû�л�������Ԥ����\n");
    } else {
        printf("\n���� %d �����ܲ���Ҫע���������⡣\n", warning_count);
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