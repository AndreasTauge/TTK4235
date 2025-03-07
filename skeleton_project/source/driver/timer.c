#include <time.h>
#include "timer.h"
#define TIME_LIMIT 3

void timer_start(Timer* t) {
    if (!t) return;
    t->start = time(NULL);
    t->active = 1;
}

int timer_expired(const Timer* t) {
    if (!t || !t->active) {
        // Timer isn't active or invalid pointer
        return 0;
    }
    return (time(NULL) - t->start) > TIME_LIMIT;
}

void timer_stop(Timer* t) {
    if (!t) return;
    t->active = 0;
}