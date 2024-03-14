#ifndef PRISONBREAK_C
#define PRISONBREAK_C

#include <stdio.h>
#include "../../include/structure.h"

#define DIM_RIDDLES_PB 2
#define DIM_OBJECTS_PB 9
#define DIM_LOCATIONS_PB 3

extern struct set prison_break;

/* handler specifici dello scenario */
void lookHandlerPB(struct mex message, int socket, struct session* current_session); 
void takeHandlerPB(struct mex message, int socket, struct session* current_session);
void useHandlerPB(struct mex message, int socket, struct session* current_session);

#endif