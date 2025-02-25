#include "elevio.h"
#include <time.h>

void set_direction(int floor, int current_floor) {
    if (current_floor > floor) {
        elevio_motorDirection(DIRN_DOWN);
    } else if (current_floor < floor) {
        elevio_motorDirection(DIRN_UP);
    }
}

void handle_floor_order() {
    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(1);
    while (elevio_obstruction()) {
        elevio_stopLamp(1);
    }
    elevio_stopLamp(0);
    nanosleep(&(struct timespec){3, 0}, NULL);
    elevio_doorOpenLamp(0);
    
}