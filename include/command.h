#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

char* commandList();

bool signupHandler(char* username, char* pwd); /* registrazione */
bool loginHandler(char* username, char* pwd); /* login */
bool endHandler(char* username); /* logout */

bool stopHandler(int nSocket); /* chiusura del server */

/*  comando choose bivalente
    primo significato: all'inizio serve per scegliere che tipo di giocatore si vuole essere
    secondo significato: se si sceglie il personaggio secondario fa scegliere in quale partita entrare */
bool chooseHandler(char* opt1, char* opt2); 

bool lookHandler(char* opt); /* opt può essere null nel caso dello scenario, altrimenti può essere una location o un oggeto */
bool takeHandler(char* obj);
bool useHandler(char* obj1, char* obj2);
bool objsHandler();

#endif

/*
Dopo aver fatto il login non deve essere più disponibile il comando register e signup (da bloccare lato server)

Aggiungere nella documentazione il comando choose e spiegarlo per bene 
*/