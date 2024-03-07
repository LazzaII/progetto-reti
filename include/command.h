#ifndef COMMAND_H
#define COMMAND_H

#include <sys/types.h>
#include <stdio.h>
#include "utils.h"

void setList(char* buffer);
void commandList(char* buffer);
void typeList(char* buffer);
void sessionList(char* buffer);
void commandSwitcher(int socket, char* message, char* type, struct session* current_session, fd_set* master);

bool signupHandler(char* username, char* pwd); /* registrazione */
bool loginHandler(char* username, char* pwd, char* err_buffer); /* login */
void endHandler(struct session* current_session, char* type, fd_set* master); /* logout */

void stopHandler(int nSocket); /* chiusura del server */ /* Non dovrebbe servire */

void startHandler(char* opt);

/*  comando choose bivalente
    primo significato: all'inizio serve per scegliere che tipo di giocatore si vuole essere
    secondo significato: se si sceglie il personaggio secondario fa scegliere in quale partita entrare */
void chooseHandler(char* opt1, char* opt2); 

void lookHandler(char* opt); /* opt può essere null nel caso dello scenario, altrimenti può essere una location o un oggeto */
void takeHandler(char* obj);
void useHandler(char* obj1, char* obj2);
void objsHandler();

#endif

/*
Dopo aver fatto il login non deve essere più disponibile il comando register e signup (da bloccare lato server)

Aggiungere nella documentazione il comando choose e spiegarlo per bene 
*/