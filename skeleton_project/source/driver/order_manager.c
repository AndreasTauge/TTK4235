#include <stdlib.h>
#include "order_manager.h"
#include "elevio.h"


int compare_floors(const void* a, const void* b) {
    Order* orderA = *(Order**)a;  // Dereference to get `Order*`
    Order* orderB = *(Order**)b;
    return orderA->floor - orderB->floor;  // Compare floors
}


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
    for (int i=0; i<*count; i++) {
        if (orders[i]->floor == floor) {
            free(orders[i]);
            for (int j = i; j < (*count) - 1; j++) {
                orders[j] = orders[j + 1];  
            }
            *count--;
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


void sort_orders(Order** orders, MotorDirection dir, int count) {
    if (dir == DIRN_UP) {
        qsort(orders, count, sizeof(Order*), compare_floors);  // sort in ascending order 
    } else if (dir == DIRN_DOWN) {
        qsort(orders, count, sizeof(Order*), compare_floors);  // sort in descending order 

        for (int i = 0; i < count / 2; i++) {
            Order* temp = orders[i];
            orders[i] = orders[count - i - 1];
            orders[count - i - 1] = temp;
        }
    }
}

