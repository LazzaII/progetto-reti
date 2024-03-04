#include <stdio.h>
#include "utils.h"

// ---------------------------
// Tipi utili
// ---------------------------

/**
 * Enumeratore per creare il boleano
*/
typedef enum { false, true } bool;

/**
 * Struttura per i messaggi
*/
struct mex
{
    char* command;
    char* opt1;
    char* opt2;

    bool ok; // se false troppi parametri
};

// ---------------------------
// FUNZIONI
// ---------------------------

/**
 * Funzione per suddividere il messaggio in comando, opzione1 e opzione2
 * @param char* message inviato da terminale
 * @return message struttura contenente la suddivisione
*/
struct mex substringMessage(char* message) {
    struct mex mex;

    mex.command = strtok(message, " ");
    mex.opt1 = strtok(NULL, " ");
    mex.opt2 = strtok(NULL, " ");

    if(strtok(NULL, " ") == NULL) { // numero di parametri giusto
        mex.ok = true;
        return mex;
    }
    else { // troppi parametri, per come è progettato il formato dei messaggi al massimo ci possono esere 1 comando e 2 opzioni
        mex.ok = false;
        return mex;
    }
}