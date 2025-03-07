#include "elevio.h"
#include "timer.h"

MotorDirection set_direction(int floor, int current_floor, MotorDirection dir) {
    if (current_floor > floor) {
        elevio_motorDirection(DIRN_DOWN);
        return DIRN_DOWN;
    } else if (current_floor < floor) {
        elevio_motorDirection(DIRN_UP);
        return DIRN_UP;
    } 
}

void handle_floor_order() {
    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(1);

}