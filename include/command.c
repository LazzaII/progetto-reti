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
void commandSwitcher(int socket, char *message, char* type, struct session* current_session, fd_set* master)
{
    struct mex substringed_mex = substringMessage(message); 
    char* buffer = NULL;

    if(substringed_mex.ok == true) {
        /* Switch dei comandi e relativa chiamata ai vari handler */
        
        /* comandi di autenticazione */
        if(strcmp(substringed_mex.command, "signup") == 0) {
            /* controllo formato messaggio */
            if(substringed_mex.opt1 == NULL || substringed_mex.opt2 == NULL){
                buffer = "Compilare correttamente i campi\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma campi non compilati correttamente");
            }

            /* chiamata e controllo dell'handler */
            if(signupHandler(substringed_mex.opt1, substringed_mex.opt2) == true) {
                buffer = "Registrazione avvenuta con successo\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Nuovo utente registrato. Username: %s", substringed_mex.opt1);
            }
            else {
                buffer = "Nome utente già presente\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma utente già presente");
            }
        }
        else if (strcmp(substringed_mex.command, "login") == 0) {
            /* controllo formato messaggio */
            if(substringed_mex.opt1 == NULL || substringed_mex.opt2 == NULL){
                buffer = "Compilare correttamente i campi\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma campi non compilati correttamente");
            }

            /* chiamata e controllo dell'handler */
            if(loginHandler(substringed_mex.opt1, substringed_mex.opt2, buffer) == true) {
                buffer = "Login avvenuto con successo\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Nuovo utente loggato. Username: %s", substringed_mex.opt1);
            }
            else {
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma %s", buffer);
            }
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
            /* controllo il messaggio inserito sia in un formato adeguato: end */
            if(substringed_mex.opt1 != NULL || substringed_mex.opt2 != NULL) {
                buffer = "Formato non riconosciuto il comando end\n\0";
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma attributi non riconosciuti");
            }

            /* se va bene il formato chiamata al comando di handler */
            endHandler(current_session, type, &master);
            printf("Chiusura da parte del client gestita correttamente"); 
        }
        /* comando non riconosciuto */
        else {
            buffer = "Comando inserito non riconosciuto\n\0";
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Invio risposta: comando non riconosciuto");
        }

    }
    /* se il messaggio è stao inviato con un formato non accettato gli rispondiamo */
    else {

        printf("Invio risposta: formato del comando inserito non valido");
    }
}

/**
 * Funzione per gestire il comando di registrazione
 * @param username nome univoco dell'utente
 * @param pwd password usata dall'utente
*/
bool signupHandler(char *username, char *pwd)
{
    return signup(username, pwd);
}

/**
 * Funzione per gestire il comando di registrazione
 * @param username nome univoco dell'utente
 * @param pwd password usata dall'utente
*/
bool loginHandler(char *username, char *pwd, char* err_buffer)
{
    return login(username, pwd, err_buffer);
}

/**
 * Funzione per gestire l'uscita dal gioco da parte dei client
 * @param session* sessione corrente
 * @param char* tipologia di utente, cambia il tipo di chiusura
*/
void endHandler(struct session* current_session, char* type, fd_set* master)
{
    char* buffer = NULL;
    /* nel caso di utente principale va chiusa la connessione ad entrambi i client */
    if(strcmp(type, "MAIN") == 0) {
        /* chiusura del socket principale*/
        close(current_session->sc_main);
        FD_CLR(current_session->sc_main, &master);
        logout(&current_session->main);

        /* invio della chiusura al secondo client */
        buffer = "Il client principale ha deciso di terminare la partita\n\0";
        send(current_session->sc_secondary, buffer, DIM_BUFFER, 0);   
        close(current_session->sc_secondary);
        FD_CLR(current_session->sc_secondary, &master);
        logout(&current_session->secondary);

        /* eliminazione fisica della sessione */
        free(current_session);
    }
    /* nel caso di utente principale va chiusa solo la connessione del client secondario */
    else {
        close(current_session->sc_secondary);
        FD_CLR(current_session->sc_secondary, &master);
        logout(&current_session->secondary);
    }
    
}
