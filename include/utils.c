#include "utils.h"

/* Variabili globali */
struct user* users_list = NULL;
struct session* sessions = NULL;

int num_sessioni = 0; /* progressivo, non cancella quando si esce*/

/**
 * Funzione per suddividere il messaggio in comando, opzione1 e opzione2
 * @param char* message inviato da terminale
 * @return struttura mex contenente la suddivisione
*/
struct mex substringMessage(char* message)
{
    struct mex mex;

    mex.command = strtok(message, " ");
    mex.opt1 = strtok(NULL, " ");
    mex.opt2 = strtok(NULL, " ");

    if(strtok(NULL, " ") == NULL) { /* numero di parametri giusto */
        mex.ok = true;
        return mex;
    }
    else { /* troppi parametri, per come è progettato il formato dei messaggi al massimo ci possono esere 1 comando e 2 opzioni */
        mex.ok = false;
        return mex;
    }
}

/**
 * Funzione per rendere il tempo corrente
 * @return timestamp (time_t ) corrente
*/
time_t getCurrentTime() 
{
    return time(NULL);
}

/**
 * Funzione per rendere il tempo rimanente, controlla anche che il tempo sia scaduto, in quel caso rende -1
 * @param time_t time il tempo da cui controllare che sia scaduto
 * @return intero contenente il valore del tempo rimanente, -1 se si è superato il tempo massimo
*/
int remainingTime(time_t startTime) 
{
    time_t current = time(NULL);
    int p = (int) difftime(current, startTime);

    return p < MAX_TIME ? MAX_TIME - p : -1;
}

/**
 * Funzione che ritorna se è presente almeno una sessione di gioco
 * @return true se è presente almeno una sessione, false altrimetni 
*/
bool gameOn()
{
    return sessions != NULL ? true : false;
}

/**
 * Funzione che rende l'utente dato il socket
 * @param socket socket dell'utente che stiamo cecando
*/
struct user* findUserFromSocket(int socket) 
{
    struct user* u;
    for (u = users_list; u; u = u->next)
    {
        if(socket == u->socket)
            return u;
    }
    return NULL;
}

/**
 * Funzione per cercare se un utente è presente nella lista degli utenti registrati
 * @param char* username nome dell'utente da cercare
 * @return user* | NULL struttura dell'utente trovato oppure NULL se non è presente
*/
struct user* findUser(char* username) 
{
    struct user* u;
    for (u = users_list; u; u = u->next)
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
    struct user* u;

    u = malloc(sizeof(struct user));
    strcpy(u->username, username);
    strcpy(u->password, pwd);
    u->logged = false;
    u->socket = -1;
    u->inGame = false;
    u->next = NULL;

    /* aggiunta in cima alla lista tanto non ci interessa l'ordine */
    if(users_list) {
        u->next = users_list;
        users_list = u;
    }
    else 
        users_list = u;
}

/**
 * Funzione per creare una nuova sessione
 * @param int socket a cui associare la sessione
 * @param set 
*/
void createSession(int socket, int pos_set) 
{   
    struct session* s;

    s = malloc(sizeof(struct session));
    s->id = ++num_sessioni;

    /* valorizzazione dello scenario */
    switch (pos_set)
    {
        case 1: /* 1 -> Prison Break*/
            s->set = prison_break;
            break;
        /* aggiungere gli altri scenari qui*/
    }

    s->start_time = time(NULL);
    s->main = findUserFromSocket(socket); 
    s->main->inGame = true; /* se si crea la sessione allora è iniziato il gioco */
    s->secondary = NULL;
    s->token_pickedUp = 0;
    s->secondary_token_pickedUp = 0;
    s->active_riddle = false;
    s->pos_riddle = 0;
    s->active_call = false;
    s->next = NULL;

    /* aggiunta in cima alla lista tanto non ci interessa l'ordine */
    if(sessions) {
        s->next = sessions;
        sessions = s;
    }
    else 
        sessions = s;
}

/**
 * Funzione per trovare la prima sessione libera
 * @param int id dello scenario che stiamo cercando
*/
struct session* firstFreeSession(int set)
{
    struct session* s = sessions;
    for (; s; s = s->next)
    {
        if(s->set.id == set && s->secondary == NULL) 
            return s; 
    }
    return NULL;
}

/**
 * Funzione per trovare la sessione associata ad un socket
 * @param int socket di cui si vuole trovare la sessione associata
 * @param char* dove viene reso il tipo di utente che sta accedendo
*/
struct session* getSession(int socket, char* type)
{
    struct session* tmp = NULL;

    for (tmp = sessions; tmp; tmp = tmp->next)
    {
        if(tmp->main && tmp->main->socket == socket){ /* giocatore principale */
            strcpy(type, "MAIN");
            break;
        }
        else if(tmp->secondary && tmp->secondary->socket == socket){ /* giocatore secondario */
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
 * Funzione per controllare se un utente ha fatto login
 * @param int socket dell'utente che stiamo cercando
 * @return true se è già loggato, false altrimenti
*/
bool userLogged(int socket) 
{
    struct user* u = findUserFromSocket(socket);
    return u != NULL ? u->logged : false;
}
