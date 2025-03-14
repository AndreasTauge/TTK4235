#pragma once
#include "elevio.h"

typedef struct {
    int floor;
    ButtonType button;
} Order;

void add_order(Order*** orders, int* count, int* capacity, int floor, ButtonType button);
void delete_order(Order** orders, int* count, int* capacity, int floor, ButtonType button);
void delete_all_orders(Order** orders, int* count);
void sort_orders(Order** orders, MotorDirection dir, int count, int currentFloor);

