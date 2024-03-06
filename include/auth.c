#include "auth.h"

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
 * @param char* username utente che sta facendo il login
 * @param pwd* password che sta inserendo per loggarsi
 * @return bool true se ok, false altrimenti (non trovato o pwd sbagliata)
*/
bool login(char* username, char* pwd) {
    struct user* u = findUser(username);

    // si controlla se abbiamo trovato l'utente
    if(u != NULL) {
        if(strcmp(pwd, u->password) == 0) {
            if(u->logged == false) {
               u->logged = true;
               return true; 
            }
            else return false; // utente già loggato in un'altra sessione
        }
        else return false; // password sbagliata
    }
    else {
        return false; // utente non trovato
    }
}

/**
 * Funzione per eseguire la registrazione dell'utente.
 * @param char* username utente che sta facendo la registrazione
 * @param pwd* password che sta inserendo per registrarsi
 * @return bool true se ok, false altrimenti (utente già inserito, pwd vuota)
*/
bool signup(char* username, char* pwd) {
    struct user* u = findUser(username);

    // si controlla se esiste un utente già presente altrimenti lo si crea
    if(u == NULL) {
       u = createUser(username, pwd);
       return true;
    }
    else {
        return false; // utente già presente
    }
}

/**
 * Funzione per eseguire il logout dalla sessione di gioco
 * @param user* utente su cui eseguire il logout
 * @param session* lista delle sessioni
*/
void logout(struct user* user, struct session* sessions) {
    struct session* current, * prec;

    // se è il client principale termina anche la sessione di gioco -> fa terminare anche l'altro client
    for (current = sessions; current->next; current = current->next)
    {
        if(strcmp(current->main->username, user->username)) {
            // invia al socket del client aggiuntivo la terminazione della chiamata
            // TODO

            // elimina la sessione corrente
            prec->next = current->next;
            free(current);
            break;
        }
        prec = current;
    }

    // client (main o aggiuntivo) messo offline
    user->logged = false;
}