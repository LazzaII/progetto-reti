#include "auth.h"

/**
 * Funzione per cercare se un utente è presente nella lista degli utenti registrati
 * @param char* username nome dell'utente da cercare
 * @return user* | NULL struttura dell'utente trovato oppure NULL se non è presente
*/
struct user* findUser(char* username) 
{
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
void createUser(char* username, char* pwd) 
{
    struct user* u = users_list;
    /* si scorre tutta la lista per aggiugere il nuovo utente */
    while (u->next) 
        u = u->next;

    u->next = malloc(sizeof(struct user));
    u->next->username = username;
    u->next->password = pwd;
    u->next->next = NULL;
}

/**
 * Funzione per trovare la sessione associata ad un socket
 * @param int socket di cui si vuole trovare la sessione associata
 * @param char* dove viene reso il tipo di utente che sta accedendo
*/
struct session* getSession(int socket, char* type)
{
    struct session* tmp = NULL;

    for (tmp = sessions; tmp->next; tmp = tmp->next)
    {
        if(tmp->sc_main == socket){ /* giocatore principale */
            strcpy(type, "MAIN");
            break;
        }
        if(tmp->sc_secondary == socket){ /* giocatore secondario */
            strcpy(type, "SEC");
            break;
        }
    }

    return tmp; /* in caso di null il giocatore ancora non ha usato il comando choose */
}

/**
 * Funzione per deallocare tutti gli utenti
*/
void deleteUsers() 
{
    struct user* tmp;
    while(users_list){
        tmp = users_list;
        users_list = users_list->next;
        free(tmp);
    }
}

/**
 * Funzione per autenticare un utente
 * @param char* username utente che sta facendo il login
 * @param pwd* password che sta inserendo per loggarsi
 * @return bool true se ok, false altrimenti (non trovato o pwd sbagliata)
*/
bool login(char* username, char* pwd, char* err_buffer) 
{
    struct user* u = findUser(username);

    /* si controlla se abbiamo trovato l'utente */
    if(u != NULL) {
        if(strcmp(pwd, u->password) == 0) {
            if(u->logged == false) {
                u->logged = true;
                return true; 
            }
            else {
                /* utente già loggato in un'altra sessione */
                err_buffer = "Utente già loggato.";
                return false;
            } 
        }
        else {
            /* password sbagliata */
            err_buffer = "Password sbagliata.";
            return false;  
        }
    }
    else {
        /* utente non trovato */
        err_buffer = "Utente non trovato.";
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
    struct session* current, * prec;

    /* se è il client principale termina anche la sessione di gioco -> fa terminare anche l'altro client */
    for (current = sessions; current->next; current = current->next)
    {
        if(strcmp(current->main->username, user->username)) {
            /* elimina la sessione corrente (in maniera logica cioè viene solo tolta dalla lista) */
            prec->next = current->next;
            break;
        }
        prec = current;
    }

    /* client (main o aggiuntivo) messo offline */
    user->logged = false;
}