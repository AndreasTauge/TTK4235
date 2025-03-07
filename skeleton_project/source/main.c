#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevator_control.h"
#include "driver/order_manager.h"
#include "driver/elevio.h"
#include "driver/timer.h"


int main(){
    elevio_init();
    int capacity=5;
    int count=0;
    Order** orders = NULL;
    orders = malloc(capacity * sizeof(Order*));
    int stopped = 0;
    int current_floor =-1;
    MotorDirection dir;
    MotorDirection old_dir;

    Timer doorTimerStop = {0, 0};
    Timer doorTimerFloor = {0, 0};

    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0);
        }}

    while(1){
        int floor = elevio_floorSensor();
        if (floor >= 0 && floor < N_FLOORS) {
            elevio_floorIndicator(floor);
            current_floor = floor;
            
        } 
        if (current_floor == -1) {
            elevio_motorDirection(DIRN_DOWN);
        } else if (count == 0) {
            elevio_motorDirection(DIRN_STOP);  // stop motor hvis ingen bestillinger
        }

        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if (stopped==0 && btnPressed && current_floor != -1) {
                    add_order(&orders, &count, &capacity, f, b);
                    sort_orders(orders, dir, count, current_floor);
                }
            }
        }
        
        if(elevio_stopButton()){
            elevio_stopLamp(1);
            elevio_motorDirection(DIRN_STOP);
            for (int i=0; i<count; i++) {
                elevio_buttonLamp(orders[i]->floor, orders[i]->button, 0);
            }
            delete_all_orders(orders, &count);
            stopped = 1;
            old_dir = dir;
        } else {
            stopped = 0;
            elevio_stopLamp(0);
        }

        if (count != 0 && current_floor != -1) {
            dir = set_direction(orders[0]->floor, current_floor, dir);
            sort_orders(orders, dir, count, current_floor);
        }

        if (stopped && floor >= 0 && floor < N_FLOORS) {
            elevio_doorOpenLamp(1);
            timer_start(&doorTimerFloor);
        } else if (timer_expired(&doorTimerFloor)) {
            elevio_doorOpenLamp(0);
        }

        if (count>0 && orders[0]->floor == floor) {  
            if (elevio_obstruction()) {
                timer_start(&doorTimerFloor);
            }
            else if (!doorTimerFloor.active) {
                timer_start(&doorTimerFloor);
                handle_floor_order();
            }
            else if (timer_expired(&doorTimerFloor)) {
                elevio_buttonLamp(orders[0]->floor, orders[0]->button, 0);
                delete_order(orders, &count, &capacity, orders[0]->floor, orders[0]->button);
                sort_orders(orders, dir, count, current_floor);
                elevio_doorOpenLamp(0);
                timer_stop(&doorTimerFloor);
            } 
            }
        else if (count != 0 && orders[0]->floor == current_floor) {
            if (old_dir == DIRN_DOWN) {
                elevio_motorDirection(DIRN_UP);
            } else if (old_dir == DIRN_UP) {
                elevio_motorDirection(DIRN_DOWN);
            }
        }

        for (int i=0; i<count; i++) {
            elevio_buttonLamp(orders[i]->floor, orders[i]->button, 1);
        
            }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
