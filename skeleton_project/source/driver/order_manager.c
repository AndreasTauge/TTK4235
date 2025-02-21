#include <stdlib.h>
#include <stdio.h>
#include "order_manager.h"
#include "elevio.h"


int stopped=0;
int current_floor;
Order** orders;

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


