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
    Order** orders = NULL; //Orders er e peker til en peker til en ønsket etasje/order
    orders = malloc(capacity * sizeof(Order*));
    int stopped = 0;
    int current_floor =-1; //initielverdi for å angi ugyldig etasje
    MotorDirection dir;
    MotorDirection old_dir;
    Timer doorTimerFloor = {0, 0};

    for(int f = 0; f < N_FLOORS; f++){ // slukker alle lys før main loop, unødvendig nå men kjørte FAT-testen med det 
        for(int b = 0; b < N_BUTTONS; b++){
            elevio_buttonLamp(f, b, 0); 
        }}

    while(1){
        int floor = elevio_floorSensor();
        if (floor >= 0 && floor < N_FLOORS) { //Setter etasjelyset for sist passerte etasje
            elevio_floorIndicator(floor);
            current_floor = floor;
            
        } 
        if (current_floor == -1) { //Setter motor retning ned når man initialiserer heisa
            elevio_motorDirection(DIRN_DOWN);
        } else if (count == 0) {
            elevio_motorDirection(DIRN_STOP);  //Stop motor hvis ingen bestillinger
        }

        for(int f = 0; f < N_FLOORS; f++){ //Legger til ordre når knapp trykkes
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if (stopped==0 && btnPressed && current_floor != -1) {
                    add_order(&orders, &count, &capacity, f, b);
                }
            }
        }
        
        if(elevio_stopButton() && current_floor != -1){ //Funksjonalitet for stopp knapp
            elevio_stopLamp(1); 
            elevio_motorDirection(DIRN_STOP); 
            for (int i=0; i<count; i++) { //Slår av alle knapp-lys
                elevio_buttonLamp(orders[i]->floor, orders[i]->button, 0);
            }
            delete_all_orders(orders, &count); 
            stopped = 1;
            old_dir = dir;
        } else { //Dersom stoppknapp ikke er trykket inn er lyset av
            stopped = 0;
            elevio_stopLamp(0);
        }

        if (count != 0 && current_floor != -1 && !doorTimerFloor.active) { //Setter motorretning til den som vil oppfylle neste order
            dir = set_direction(orders[0]->floor, current_floor, dir);
        }

        if (stopped && floor >= 0 && floor < N_FLOORS) { //Setter på dør lyset og starter dør-timer om stopp knapp trykket inn i en etasje
            elevio_doorOpenLamp(1);
            timer_start(&doorTimerFloor);
        } else if (timer_expired(&doorTimerFloor)) { //Slukker dør lyset og stopper dør-timer når timer har gått ut
            elevio_doorOpenLamp(0);
            timer_stop(&doorTimerFloor);
        }

        if (elevio_obstruction()) { 
            timer_start(&doorTimerFloor);
        }

        if (count>0) { 
        for (int i=0; i<count; i++) { //Funksjonalitet for å ikke stoppe i etasje dersom etasjen ikke er prioritert (i henhold til FAT-test H3)
            if (orders[i]->floor == floor) {   // gå til neste iterasjon om knappen trykt ikke er i samme retning som heisen skal (så den ikke stopper)
                if (orders[i]->button == BUTTON_HALL_UP && dir == DIRN_DOWN) {
                    continue;
                } else if (orders[i]->button == BUTTON_HALL_DOWN && dir==DIRN_UP) {
                    continue;
                }
                timer_start(&doorTimerFloor);
                handle_floor_order();
                elevio_buttonLamp(orders[i]->floor, orders[i]->button, 0);
                delete_order(orders, &count, &capacity, orders[i]->floor, orders[i]->button);
                
                if (timer_expired(&doorTimerFloor)) { // når 3 sekunder har gått lukkes døren
                    elevio_doorOpenLamp(0);
                    timer_stop(&doorTimerFloor);
                } 
                }
            else if (orders[i]->floor == current_floor && count == 1) {  // logikk for å sikre at heisen kan ta ordre ned/opp til forrige etasje etter stoppknapp er trykket
                if (old_dir == DIRN_DOWN) {
                    elevio_motorDirection(DIRN_UP);
                } else if (old_dir == DIRN_UP) {
                    elevio_motorDirection(DIRN_DOWN);
                }
            }}
    }

        for (int i=0; i<count; i++) {
            elevio_buttonLamp(orders[i]->floor, orders[i]->button, 1);
        
            }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
