#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include "utils.h"

// lista dei comandi disponibili
extern const char* COMMAND_LIST = "Lista dei comandi disponibili:"
           "\t- look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggeto/location in questione.\n"
           "\t- take obj: raccoglie l'oggetto <obj> indicato come parametro."
           "\t- use obj1 [obj2]: permette di utilizzare l'oggeto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggeto <obj2>."
           "\t- objs: rende la lista degli oggetti raccolti."
           "\t- end: termina la partita ed esce dal gioco.";

void commandSwitcher(int socket, char* message, char* type);

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