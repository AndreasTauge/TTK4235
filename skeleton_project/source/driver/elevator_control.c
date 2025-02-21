#include "elevio.h"
void set_direction(int floor, int current_floor) {
    if (current_floor > floor) {
        elevio_motorDirection(DIRN_DOWN);
    } else if (current_floor < floor) {
        elevio_motorDirection(DIRN_UP);
    }
}