#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>

#define MAX_TIME 480 /* tempo massimo in secondi */

struct mex substringMessage(char* message);
time_t getCurrentTime();
int remainingTime(time_t time);

#endif