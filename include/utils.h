#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include "auth.h"

#define MAX_TIME 480 /* tempo massimo in secondi */

/* Tipi utili */
typedef enum { false, true } bool;

struct mex
{
    char* command;
    char* opt1;
    char* opt2;

    bool ok; /* se false troppi parametri */
};

/* Dichiarazione funzioni */
struct mex substringMessage(char* message);
time_t getCurrentTime();
int remainingTime(time_t time);
bool gameOn();

#endif