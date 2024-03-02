#include "utility.c"

/**
 * Struttura degli utenti
*/
struct user
{
    // dati utente
    int id;
    char* username;
    char* password;

    // dati per la sessione
    bool logged;
};

/**
 * Struttura per la sessione collegata ad uno scenario e un utente, più l'utente aggiuntivo
*/
struct session
{
    // dati della sessione
    int id;
    struct set set; 
    // client
    // client aggiuntivo
};

/**
 * Struttura per i vari scenari, conterrà la lista di tutte le location e oggetti
*/
struct set 
{
    // dati dello scenario
    int id;
    char* name;

    // location dello scenario
    struct location* locations;

    // oggetti dello scenario che si possono prendere subito
    struct object* objs;
};

/**
 * Struttura per le varie locazioni dello scenraio, conterrà la lista di tutti gli oggetti in quella location
*/
struct location
{
    // dati della location
    int id;
    char* name;

    // oggetti della location
    struct object* objs;
};

/**
 * Struttura per i vari oggetti
*/
struct object
{
    // dati degli oggetti
    int id;
    char* name;
    bool pickedUp;
    bool used; // se è stato utilizzato e non può essere più utilizzato (ex. use telefono sapone = bomba dopo non si può più usare telefono e sapone)
    bool found; // se la location è stata usata, per evitare chiamate a caso senza "scoprire" la stanza
};



