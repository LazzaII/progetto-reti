#ifndef COMMAND_H
#define COMMAND_H

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "utils.h"
#include "../scenari/prison_break/prison_break.h"

void setList(char* buffer);
void commandList(char* buffer);
void commandSwitcher(int socket, char* message, char* type, struct session* current_session, fd_set* master);

void signupHandler(struct mex message, int socket); /* registrazione */
void loginHandler(struct mex message, int socket); /* login */
void endHandler(struct session* current_session, char* type, fd_set* master); /* logout */

void startHandler(struct mex message, int socket); /* formato del comando start cambiato, start num tipo*/

void lookHandler(struct mex message, int socket); /* opt può essere null nel caso dello scenario, altrimenti può essere una location o un oggeto */
void takeHandler(struct mex message, int socket);
void useHandler(struct mex message, int socket);
void objsHandler(struct mex message, int socket, struct session* current_session);

void sendInfos(struct session* current_session, int socket); /* al termine di ogni messaggio si inviano le info sulla partita, tempo rimanente e token raccolti*/

#endif

/*
Dopo aver fatto il login non deve essere più disponibile il comando register e signup (da bloccare lato server)

Aggiungere nella documentazione il comando choose e spiegarlo per bene 
*/