#include <stdio.h>
#include <string.h>
#include "structure.c"
#include "utils.h"

// Variabili globali
struct user* users_list = NULL;

/**
 * Funzione per cercare se un utente è presente nella lista degli utenti registrati
 * @param char* username nome dell'utente da cercare
 * @return user* | NULL struttura dell'utente trovato oppure NULL se non è presente
*/
struct user* findUser(char* username) {
    struct user* u;
    for (u = users_list; u->next; u = u->next)
    {
        if(strcmp(username, u->username) == 0)
            return u;
    }
    return NULL;
}

/**
 * Funzione per creare un utente, se l'utente è gia presente rende NULL (stesso nome utente)
 * @param char* username nome dell'utente da aggiungere
 * @param char* pwd password dell'utente da aggiungere
 * @return user* | NULL struttura appena creata oppure NULL se già presente
*/
struct user* createUser(char* username, char* pwd) {
    struct user* u, *toAdd;
    for (u = users_list; u->next; u = u->next)
    {
        if(strcmp(username, u->username) == 0)
            return NULL;
    }
    // se non viene trovato si valorizza la nuova struttura
    toAdd->username = username;
    toAdd->password = pwd;
    u->next = toAdd;
    return toAdd;
}

/**
 * Funzione per autenticare un utente
*/
bool login(char* username, char* pwd) {
    struct user* u, *toAdd;
    for (u = users_list; u->next; u = u->next)
    {
        if(strcmp(username, u->username) == 0)
            break;           
    }

    // si controlla se siamo arrivati a fine del for o se abbiamo trovato l'utente
    if(u->next) {
        if(strcmp(pwd, u->password) == 0) {
            if(u->logged == false) {
               u->logged = true;
               return true; 
            }
            else return false; // utente già loggato in un'altra sessione
        }
        else return false // password sbagliata
    }
    else {
        return false; // utente non trovato
    }

}