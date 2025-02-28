#include "elevio.h"
#include "timer.h"

MotorDirection set_direction(int floor, int current_floor) {
    if (current_floor > floor) {
        elevio_motorDirection(DIRN_DOWN);
    } else if (current_floor < floor) {
        elevio_motorDirection(DIRN_UP);
    } else {
        elevio_motorDirection(DIRN_STOP);
    }
}

void handle_floor_order() {
    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(1);
    while (elevio_obstruction()) {
        elevio_stopLamp(1);
    }
    elevio_stopLamp(0);
}