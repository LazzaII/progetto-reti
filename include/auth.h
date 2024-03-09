#ifndef AUTH_H
#define AUTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "utils.h"

/* Autenticazione  */
bool login(char* username, char* pwd, int socket, char* err_buffer);
bool signup(char* username, char* pwd);
void logout(struct user* user);

#endif