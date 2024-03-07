#include "utils.h"

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

    return p < MAX_TIME ? p : -1;
}

/**
 * Funzione che ritorna se è presente almeno una sessione di gioco
 * @return true se è presente almeno una sessione, false altrimetni 
*/
bool gameOn()
{
    return sessions != NULL ? true : false;
}
