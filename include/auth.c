#include "auth.h"

/**
 * Funzione per autenticare un utente
 * @param char* username utente che sta facendo il login
 * @param pwd* password che sta inserendo per loggarsi
 * @param socket con cui si sta collegando
 * @return bool true se ok, false altrimenti (non trovato o pwd sbagliata)
*/
bool login(char* username, char* pwd, int socket, char* err_buffer) 
{
    struct user* u = findUser(username);

    /* si controlla se abbiamo trovato l'utente */
    if(u != NULL) {
        if(strcmp(pwd, u->password) == 0) {
            if(u->logged == false) {
                u->logged = true;
                u->socket = socket;
                return true; 
            }
            else {
                /* utente già loggato in un'altra sessione */
                strcpy(err_buffer, "Utente già loggato.\n");
                return false;
            } 
        }
        else {
            /* password sbagliata */
            strcpy(err_buffer, "Password sbagliata.\n");
            return false;  
        }
    }
    else {
        /* utente non trovato */
        strcpy(err_buffer, "Utente non trovato.\n");
        return false; 
    }
}

/**
 * Funzione per eseguire la registrazione dell'utente.
 * @param char* username utente che sta facendo la registrazione
 * @param pwd* password che sta inserendo per registrarsi
 * @return bool true se ok, false altrimenti (utente già inserito, pwd vuota)
*/
bool signup(char* username, char* pwd) 
{
    struct user* u = findUser(username);

    /* si controlla se esiste un utente già presente altrimenti lo si crea */
    if(u == NULL) {
        createUser(username, pwd);
        return true;
    }
    else {
        return false; /* utente già presente */
    }
}

/**
 * Funzione per eseguire il logout dalla sessione di gioco
 * @param user* utente su cui eseguire il logout
*/
void logout(struct user* user) 
{
    struct session* current, * prec = NULL;

    /* se è il client principale termina anche la sessione di gioco -> fa terminare anche l'altro client */
    for (current = sessions; current; current = current->next)
    {
        if(strcmp(current->main->username, user->username)) {
            /* elimina la sessione corrente (in maniera logica cioè viene solo tolta dalla lista) */
            if(prec)
                prec->next = current->next;
            break;
        }
        prec = current;
    }

    if(user) { /* non il miglior modo ma almeno non gestiamo due casi diversi per loggati e non loggati */
        /* client (main o aggiuntivo) messo offline */
        user->logged = false;
        user->socket = -1;
        user->inGame = false;
    }
}