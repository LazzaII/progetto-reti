#ifndef STRUCTURE_C
#define STRUCTURE_C

#include <time.h>

#define MAX_LOC_SET 3
#define MAX_OBJ_SET 9
#define MAX_RIDDLES_SET 2
#define MAX_OBJ_LOC 2
#define MAX_CLIENT 20 /* numero massimo di client che si possono connettere */
#define DIM_BUFFER 1024 /* grandezza buffer di client e server */

/**
 * Tipo bool definito visto che non è presente di base in c
*/
typedef enum { false, true } bool;

/**
 * Strtuttura per i messaggi
*/
struct mex
{
    char* command;
    char* opt1;
    char* opt2;

    bool ok; /* se false troppi parametri */
};

/**
 * Struttura degli utenti
*/
struct user
{
    /* dati utente */
    char username[255];
    char password[255];

    /* dati per la sessione */
    bool logged;
    int socket;
    bool inGame;

    /* lista utenti */
    struct user* next;
};

/**
 * Struttura per gli enigmi
*/
struct riddle 
{
    char* description;
    char* solution;
    bool solved;
    char* mex_sol;
};

/**
 * Struttura per i vari oggetti
*/
struct object
{
    /* dati degli oggetti */
    char* name;
    char* description;
    char* use_description;

    bool pickedUp; /* se è stato raccolot */
    bool used; /* se è stato utilizzato e non può essere più utilizzato (ex. use telefono sapone = bomba dopo non si può più usare telefono e sapone) */
    bool found; /* se la location è stata scoperta, per evitare chiamate a caso senza "scoprire" la stanza */
    bool is_token; /* se rappresenta un token */
    bool is_secondary_token; /* se rappresenta un token */

    int riddle; /* nel caso l'oggetto sia bloccato da un enigma */
};

/**
 * Struttura per le varie locazioni dello scenraio, conterrà la lista di tutti gli oggetti in quella location
*/
struct location
{
    /* dati della location */
    char* name;
    char* description;

    /* oggetti della location */
    int objs[MAX_OBJ_LOC];
};

/**
 * Struttura per i vari scenari, conterrà la lista di tutte le location e oggetti
*/
struct set 
{
    /* dati dello scenario */
    int id; /* codice da digitare da parte del client */
    char* name;
    char* description; /* per look senza paramentri */

    /* location dello scenario */
    struct location locations[MAX_LOC_SET];

    /* oggetti dello scenario su cui si può fare subito take subito */
    struct object objects[MAX_OBJ_SET];

    /* quiz dello scenario */
    struct riddle riddles[MAX_RIDDLES_SET];
};

/**
 * Struttura per la sessione collegata ad uno scenario e un utente, più l'utente aggiuntivo
*/
struct session
{
    /* dati della sessione */
    int id; /* per verbose del server */
    struct set set; /* scenario scelto */
    time_t start_time; /* tempo di inizio*/

    /* client principale */
    struct user* main; 

     /* client aggiuntivo */
    struct user* 
    secondary;

    /* numero di token raccolti */
    int token_pickedUp; 
    int secondary_token_pickedUp;

    /* var per quando si sta aspettando risposta da un indovinello*/
    bool active_riddle;
    int pos_riddle;

    /* var per quando si è usato il telefono e si sta aspettando l'altro giocatore */
    bool active_call;

    /* lista delle sessioni */
    struct session* next;
};

#endif

/**
 * TOKEN ASSEGNATI
 * sol 1 (principale)
 * - telefono
 * - sapone
 * - bomba (da tel e sapone)
 * 
 * sol 2 (secondaria)
 * - monete1 
 * - monete2
 * - monete3
 * (Dipende da quante ne chiede il client aggiuntivo)
*/