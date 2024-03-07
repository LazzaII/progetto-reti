#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <string.h>
#include "structure.c"
#include "utils.h"


/* Var globali */
extern struct user* users_list;
extern struct session* sessions;

/* Utility */
struct user* findUser(char* username);
struct user* createUser(char* username, char* pwd);
struct session* getSession(int socket, char* type);

/* Autenticazione  */
bool login(char* username, char* pwd);
bool signup(char* username, char* pwd);
void logout(struct user* user);

#endif