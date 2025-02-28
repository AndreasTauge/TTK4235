#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevator_control.h"
#include "driver/order_manager.h"
#include "driver/elevio.h"


int main(){
    elevio_init();
    int capacity=5;
    int count=0;
    Order** orders = NULL;
    orders = malloc(capacity * sizeof(Order*));
    int stopped = 0;
    int current_floor =0;

    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0);
        }}
/*
    add_order(&orders, &count, &capacity, 2, BUTTON_CAB);
    add_order(&orders, &count, &capacity, 3, BUTTON_CAB);
    add_order(&orders, &count, &capacity, 1, BUTTON_CAB);
*/
    while(1){
        int floor = elevio_floorSensor();
        if (floor >= 0 && floor < N_FLOORS) {
            elevio_floorIndicator(floor);
            current_floor = floor;
        }

        if (count == 0) {
            elevio_motorDirection(DIRN_STOP);  // stop motor hvis ingen bestillinger
        }

        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if (stopped==0 && btnPressed) {
                    add_order(&orders, &count, &capacity, f, b);
                }
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            delete_all_orders(orders, &count);
            stopped = 1;
        }

        if (count != 0) {
            MotorDirection dir = set_direction(orders[0]->floor, current_floor);
            //sort_orders(orders, count, dir);
        }

        if (count>0 && orders[0]->floor == current_floor) {
            handle_floor_order();
            delete_order(orders, &count, &capacity, orders[0]->floor, orders[0]->button);
        }

        for (int i=0; i<count; i++) {
            elevio_buttonLamp(orders[i]->floor, orders[i]->button, 1);
        
            }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
