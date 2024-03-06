#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <string.h>
#include "structure.c"
#include "utils.h"

// Global var
extern struct user* users_list;

// Utility
struct user* findUser(char* username);
struct user* createUser(char* username, char* pwd);

// Autenticazione 
bool login(char* username, char* pwd);
bool signup(char* username, char* pwd);
void logout(struct user* user, struct session* sessions);

#endif