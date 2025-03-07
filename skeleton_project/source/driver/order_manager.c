#include <stdlib.h>
#include <stdio.h>
#include "order_manager.h"
#include "elevio.h"

void add_order(Order*** orders, int* count, int* capacity, int floor, ButtonType button) {
    if (*count >= *capacity) {
        *capacity *= 2;
        *orders = realloc(*orders, (*capacity) * sizeof(Order*));
        if (*orders == NULL) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
    }

    (*orders)[*count] = malloc(sizeof(Order));
    if ((*orders)[*count] == NULL) {
        printf("Order allocation failed!\n");
        exit(1);
    }

    (*orders)[*count]->floor = floor;
    (*orders)[*count]->button = button;
    (*count)++;
}


void delete_order(Order** orders, int* count, int* capacity, int floor, ButtonType button) {
    for (int i=0; i<*count;) {
        if (orders[i]->floor == floor) {
            free(orders[i]);
            for (int j = i; j < (*count) - 1; j++) {
                orders[j] = orders[j + 1];  
            }
            orders[*count - 1] = NULL; 
            (*count)--;
        } else {
            i++;
        }
    }
}

void delete_all_orders(Order** orders, int* count) {
    for (int i=0; i<*count; i++) {
        free(orders[i]);
        orders[i] = NULL;
    }
    *count=0;
};


int compare_floors(const void* a, const void* b) {
    Order* orderA = *(Order**)a;
    Order* orderB = *(Order**)b;
    return orderA->floor - orderB->floor;
}

void sort_orders(Order** orders, int count, int currentFloor, MotorDirection dir) {
    if (count <= 1) return; // No need to sort 0 or 1 order


    Order** upList   = malloc(count * sizeof(Order*));
    Order** downList = malloc(count * sizeof(Order*));
    if (!upList || !downList) {
        return;
    }

    int upCount = 0;
    int downCount = 0;

    for (int i = 0; i < count; i++) {
        if (orders[i]->floor >= currentFloor) {
            upList[upCount++] = orders[i];
        } else {
            downList[downCount++] = orders[i];
        }
    }

    qsort(upList,   upCount,   sizeof(Order*), compare_floors);
    qsort(downList, downCount, sizeof(Order*), compare_floors);

    int idx = 0;
    if (dir == DIRN_UP) {
        for (int i = 0; i < upCount; i++) {
            orders[idx++] = upList[i];
        }
        for (int i = downCount - 1; i >= 0; i--) {
            orders[idx++] = downList[i];
        }
    }
    else if (dir == DIRN_DOWN) {
        for (int i = downCount - 1; i >= 0; i--) {
            orders[idx++] = downList[i];
        }
        for (int i = 0; i < upCount; i++) {
            orders[idx++] = upList[i];
        }
    }
    else {
        qsort(orders, count, sizeof(Order*), compare_floors);
    }

    free(upList);
    free(downList);
}
