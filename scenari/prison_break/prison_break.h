#ifndef PRISONBREAK_C
#define PRISONBREAK_C

#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include "../../include/structure.h"
#include "../../include/auth.h"

#define DIM_RIDDLES_PB 2
#define DIM_OBJECTS_PB 9
#define DIM_LOCATIONS_PB 3
#define MAX_TOKEN_PB 3

extern struct set prison_break;

/* handler specifici dello scenario */
void lookHandlerPB(struct mex message, int socket, struct session* current_session); 
void takeHandlerPB(struct mex message, int socket, struct session* current_session);
bool useHandlerPB(struct mex message, int socket, struct session* current_session);

void riddleHandlerPB(struct mex message, struct session* current_session);
void callHandlerPB(struct mex message, struct session* current_session, char* type, fd_set* master);

#endif