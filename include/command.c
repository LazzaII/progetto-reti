#include "command.h"

/**
 * Funzione per valorizzare buffer con la lista degli scenari
 * @param char* buffer da valorizzare
*/
void setList(char* buffer)
{
    buffer = "Lista degli scenari disponibili, usare il comando start <num> per avviare lo scenario:\n"
            "\t- 1: Prison Break\n\0";
}

/**
 * Funzione per valorizzare buffer con la lista dei comandi
 * @param char* buffer da valorizzare
*/
void commandList(char* buffer) 
{
    buffer = "Lista dei comandi disponibili:\n"
            "\t- look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggeto/location in questione.\n"
            "\t- take obj: raccoglie l'oggetto <obj> indicato come parametro.\n"
            "\t- use obj1 [obj2]: permette di utilizzare l'oggeto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggeto <obj2>.\n"
            "\t- objs: rende la lista degli oggetti raccolti.\n"
            "\t- end: termina la partita ed esce dal gioco.\n\0";
}

/**
 * Funzione per valorizzare buffer con la tipologia di giocatori disponibili
 * @param char* buffer da valorizzare
*/
void typeList(char* buffer)
{
    buffer = "Scegli il tipo di giocatore che vuoi essere, usare il comando choose tipo <num>:\n"
            "\t- 1: Principale\n"
            "\t- 2: Secondario\n\0";
}

/**
 * Funzione per valorizzare buffer con la lista delle sessioni dove il personaggio seconadrio può entrare
 * @param char* buffer da valorizzare
*/
void sessionList(char* buffer)
{
    struct session* current;
    char* tmp = NULL;
    buffer = "Scegli il tipo di giocatore che vuoi essere, usare il comando choose session <num>\n";

    /* se è il client principale termina anche la sessione di gioco -> fa terminare anche l'altro client */
    for (current = sessions; current->next; current = current->next)
    {
        // TODO: da rivedere
        sprintf(tmp, "\t- %d: %s\n\0", current->id, current->set.name);
        strcat(buffer, tmp);
    }
}

/**
 * Funzione di switch dei comandi ai rispettivi handler, tramite socket risponde direttamente al client corretto
 * 
*/
void commandSwitcher(int socket, char *message, char* type)
{
    struct mex substringed_mex = substringMessage(message); 

    if(substringed_mex.ok == true) {
        /* Switch dei comandi e relativa chiamata ai vari handler */
        /* comandi di autenticazione */
        if(strcmp(substringed_mex.command, "signup") == 0) {

        }
        else if (strcmp(substringed_mex.command, "login") == 0) {
        
        }
        else if (strcmp(substringed_mex.command, "choose") == 0) {
        
        }
        /* comandi di gioco */
        else if (strcmp(substringed_mex.command, "look") == 0) {
        
        }
        else if (strcmp(substringed_mex.command, "take") == 0) {
        
        }
        else if (strcmp(substringed_mex.command, "use") == 0) {
        
        }
        else if (strcmp(substringed_mex.command, "objs") == 0) {
        
        }
        /* comando di terminazione */
        else if (strcmp(substringed_mex.command, "end") == 0) {
        
        }
        /* comando non riconosciuto */
        else {

            printf("Invio risposta: comando non riconosciuto");
        }

    }
    /* se il messaggio è stao inviato con un formato non accettato gli rispondiamo */
    else {

        printf("Invio risposta: formato del comando inserito non valido");
    }
}
