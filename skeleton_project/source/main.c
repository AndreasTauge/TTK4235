#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "order_manager.h"
#include "elevator_control.h"


int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);
    int capacity=5;
    int count=0;
    Order** orders = NULL;
    orders = malloc(capacity * sizeof(Order*));

    while(1){
        int floor = elevio_floorSensor();

        if (floor >= 0 && floor < N_FLOORS) {
            elevio_floorIndicator(floor);
        }

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }

        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                add_order(&orders, &count, &capacity, 2, 1);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        for (int i=0; i<count; i++) {
            set_direction(orders[i]->floor, floor);
            if (orders[i]->floor == floor) {
                elevio_motorDirection(DIRN_STOP);
                delete_order(&orders, &count, &capacity, orders[i]->floor, orders[i]->button);
            }
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
