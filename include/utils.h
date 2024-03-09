#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <time.h>
#include "auth.h"
#include "structure.h"
#include "../scenari/prison_break/prison_break.h"

#define MAX_TIME 600 /* tempo massimo in secondi per tutti gli scenari*/
#define DIM_BUFFER 1024 /* grandezza buffer di client e server */

/* Variabili globali */
extern struct user* users_list;
extern struct session* sessions;

/* Dichiarazione funzioni */
struct mex substringMessage(char* message);
time_t getCurrentTime();
int remainingTime(time_t time);
bool gameOn();
struct user* findUserFromSocket(int socket);
struct user* findUser(char* username); 
void createUser(char* username, char* pwd);
void createSession(int socket, int pos_set);
struct session* firstFreeSession(int set);
struct session* getSession(int socket, char* type);
void deleteUsers();
bool userLogged(int socket);

#endif