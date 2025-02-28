#include <time.h>

#define TIME_LIMIT 3

static int doorTimer = -1;
static int doorTimer2 = -1;

void timer_start(int timer_num) {
    if (timer_num == 1) {
        doorTimer = time(NULL);
    } else if (timer_num == 2) {
        doorTimer2 = time(NULL);
    }

}

int timer_expired(int timer_num) {
	if (doorTimer == -1 && timer_num == 1) return 0;
    else if (doorTimer2 == -1 && timer_num == 2) return 0;
	else return (time(NULL) - doorTimer > TIME_LIMIT);
}