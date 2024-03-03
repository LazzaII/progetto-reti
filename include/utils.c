#include "structure.c"
#include "utils.h"

// ---------------------------
// ENUM
// ---------------------------

/**
 * Enumeratore per creare il boleano
*/
typedef enum { false, true } bool;

// ---------------------------
// FUNZIONI
// ---------------------------

/**
 * Funzione per suddividere il messaggio in comando, opzione1 e opzione2
 * @param char* message inviato da terminale
 * @return message struttura contenente la suddivisione
*/
struct message substringMessage(char* message) {
    struct mex mex;

    mex.command = strtok(buffer, " ");
    mex.opt1 = strtok(NULL, " ");
    mex.opt2 = strtok(NULL, " ");

    return mex;
}
