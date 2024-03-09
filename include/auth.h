#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "utils.h"

/* Utility */
struct user* findUser(char* username);
void createUser(char* username, char* pwd);
struct session* getSession(int socket, char* type);
void deleteUsers();

/* Autenticazione  */
bool login(char* username, char* pwd, char* err_buffer);
bool signup(char* username, char* pwd);
void logout(struct user* user);

#endif