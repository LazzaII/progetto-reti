#ifndef STRUCTURE_C
#define STRUCTURE_C

#include "utils.c"
#include <time.h>

/**
 * Struttura degli utenti
*/
struct user
{
    // dati utente
    char* username;
    char* password;

    // dati per la sessione
    bool logged;

    // lista utenti
    struct user* next;
};

/**
 * Struttura per la sessione collegata ad uno scenario e un utente, più l'utente aggiuntivo
*/
struct session
{
    // dati della sessione 
    int id; // utile per l'associazione con il client aggiuntivo
    struct set set; // scenario scelto
    time_t time; // tempo per lo scenario
    int sc_main; // client principale
    struct user* main; 
    int sc_secondary; // client aggiuntivo
    struct user* secondary;

    int token_pickedUp; // numero di token raccolti

    // lista delle sessioni
    struct session* next;
};

/**
 * Struttura per i vari scenari, conterrà la lista di tutte le location e oggetti
*/
struct set 
{
    // dati dello scenario
    int id; // codice da digitare da parte del client
    char* name;
    char* description;

    // location dello scenario
    struct location* locations;

    // oggetti dello scenario su cui si può fare subito take subito
    struct object* objs;
};

/**
 * Struttura per le varie locazioni dello scenraio, conterrà la lista di tutti gli oggetti in quella location
*/
struct location
{
    // dati della location
    char* name;
    char* description;

    // oggetti della location
    struct object** objs;
};

/**
 * Struttura per i vari oggetti
*/
struct object
{
    // dati degli oggetti
    char* name;
    char* description;
    char* use_description;

    bool pickedUp;
    bool used; // se è stato utilizzato e non può essere più utilizzato (ex. use telefono sapone = bomba dopo non si può più usare telefono e sapone)
    bool found; // se la location è stata scoperta, per evitare chiamate a caso senza "scoprire" la stanza
    bool is_token;

    struct riddle* riddle; // nel caso l'oggetto sia bloccato da un enigma
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
 * TOKEN ASSEGNATI
 * sol 1
 * - telefono
 * - sapone
 * - bomba (da tel e sapone)
 * 
 * sol 2 
 * - monete1 
 * - monete2
 * - monete3
 * (Dipende da quante ne chiede il client aggiuntivo)
*/

#endif