#include "elevio.h"
#include "timer.h"

MotorDirection set_direction(int floor, int current_floor, MotorDirection dir) {
    if (current_floor > floor) {
        elevio_motorDirection(DIRN_DOWN);
        return DIRN_DOWN;
    } else if (current_floor < floor) {
        elevio_motorDirection(DIRN_UP);
        return DIRN_UP;
    } else if (current_floor == floor) {
        if (dir == DIRN_UP) {
            elevio_motorDirection(DIRN_DOWN);
            return DIRN_DOWN;
        } else if (dir == DIRN_DOWN) {
            elevio_motorDirection(DIRN_UP);
            return DIRN_UP;
        }
        }
    else {
        elevio_motorDirection(DIRN_STOP);
        return DIRN_STOP;
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