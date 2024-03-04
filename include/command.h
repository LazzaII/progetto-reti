#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

bool signup_handler(char* username, char* pwd); // registrazione
bool login_handler(char* username, char* pwd); // login
bool end_handler(char* username); // logout

bool stop_handler(int nSocket); // chiusura del server

// comando choose bivalente
// primo significato: all'inizio serve per scegliere che tipo di giocatore si vuole essere
// secondo significato: se si sceglie il personaggio secondario fa scegliere in quale partita entrare
bool choose_handler(char* opt1, char* opt2); 

bool look_handler(char* opt = NULL); // opt può essere null nel caso dello scenario, altrimenti può essere una location o un oggeto
bool take_handler(char* obj);
bool drop_handler(char* obj);
bool use_handler(char* obj1, char* obj2 = NULL);
bool objs_handler();
#endif

/*
Dopo aver fatto il login non deve essere più disponibile il comando register e signup (da bloccare lato server)

Aggiungere nella documentazione il comando choose e spiegarlo per bene 
*/