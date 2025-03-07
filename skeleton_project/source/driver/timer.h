#include <time.h>

typedef struct {
    time_t start;
    int active;
} Timer;

void timer_start(Timer* t);
int timer_expired(const Timer* t);
void timer_stop(Timer* t);