#include "command.h"


/**
 * Funzione per valorizzare buffer con la lista degli scenari
 * @param char* buffer da valorizzare
*/
void setList(char* buffer)
{
    strcpy(buffer, 
            "Lista degli scenari disponibili, usare il comando start <num> per avviare lo scenario:\n"
            "\t- 1: Prison Break\n");
}

/**
 * Funzione per valorizzare buffer con la lista dei comandi
 * @param char* buffer da valorizzare
*/
void commandList(char* buffer) 
{
    strcpy(buffer, 
            "Lista dei comandi disponibili:\n"
            "\t- look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggeto/location in questione.\n"
            "\t- take obj: raccoglie l'oggetto <obj> indicato come parametro.\n"
            "\t- use obj1 [obj2]: permette di utilizzare l'oggeto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggeto <obj2>.\n"
            "\t- objs: rende la lista degli oggetti raccolti.\n"
            "\t- end: termina la partita ed esce dal gioco.\n");
}

/**
 * Funzione per valorizzare buffer con la tipologia di giocatori disponibili
 * @param char* buffer da valorizzare
*/
void typeList(char* buffer)
{
    strcpy(buffer, 
            "Scegli il tipo di giocatore che vuoi essere, usare il comando choose tipo <num>:\n"
            "\t- 1: Principale\n"
            "\t- 2: Secondario\n");
}

/**
 * Funzione per valorizzare buffer con la lista delle sessioni dove il personaggio seconadrio può entrare
 * @param char* buffer da valorizzare
*/
void sessionList(char* buffer)
{
    struct session* current;
    char* tmp = NULL;
    strcpy(buffer, "Scegli il tipo di giocatore che vuoi essere, usare il comando choose session <num>\n");

    /* se è il client principale termina anche la sessione di gioco -> fa terminare anche l'altro client */
    for (current = sessions; current->next; current = current->next)
    {
        /* TODO: da rivedere */ 
        sprintf(tmp, "\t- %d: %s\n%c", current->id, current->set.name, '\0');
        strcat(buffer, tmp);
    }
}

/**
 * Funzione di switch dei comandi ai rispettivi handler, tramite socket risponde direttamente al client corretto
 * @param int socket a cui inviare le comunicazioni
 * @param char* messaggio ricevuto dal client
 * @param char* tipo di personaggio che ha inviato il messaggio
 * @param session* struttura contenente la sessione di riferimento del client
 * @param fd_set* lista principale dei descrittori
*/
void commandSwitcher(int socket, char *message, char* type, struct session* current_session, fd_set* master)
{
    struct mex substringed_mex = substringMessage(message); 
    char buffer[DIM_BUFFER];

    /* Switch dei comandi con relativa chiamata ai vari handler */
    if(substringed_mex.ok == true) {
        /* comandi di autenticazione */
        if(strcmp(substringed_mex.command, "signup") == 0) {
            /* se l'utente ha già fatto login non si può usare il comando signup */
            if(strcmp(substringed_mex.command, "MAIN") == 0 && current_session->main->logged) {
                strcpy(buffer, "Partita in corso, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Partita in corso, comando non disponibile");
            } 
            else if(current_session->secondary->logged) {
                strcpy(buffer, "Partita in corso, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Partita in corso, comando non disponibile");
            }
            else /* chiamata all'handler */
                signupHandler(substringed_mex, socket);
        }
        else if (strcmp(substringed_mex.command, "login") == 0) {
            /* se l'utente ha già fatto login non si può usare il comando login */
            if(strcmp(substringed_mex.command, "MAIN") == 0 && current_session->main->logged) {
                strcpy(buffer, "Partita in corso, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Partita in corso, comando non disponibile");
            } 
            else if(current_session->secondary->logged) {
                strcpy(buffer, "Partita in corso, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Partita in corso, comando non disponibile");
                
            }
            else /* chiamata all'handler */
                loginHandler(substringed_mex, socket);
        }
        /* comandi di inizio gioco*/
        else if (strcmp(substringed_mex.command, "start") == 0) {
        
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
            /* controllo che il giocatore che ha usato objs sia il principale */
            if(strcmp(type, "SEC")) {
                strcpy(buffer, "Comando non valido da giocatore secondario\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando non valido da giocatore secondario");
            }
            else 
                objsHandler(substringed_mex, socket, current_session);

        }
        /* comando di terminazione */
        else if (strcmp(substringed_mex.command, "end") == 0) {
            /* controllo il messaggio inserito sia in un formato adeguato: end */
            if(substringed_mex.opt1 != NULL || substringed_mex.opt2 != NULL) {
                strcpy(buffer, "Formato non riconosciuto il comando end\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito correttamente ma attributi non riconosciuti");
                return;
            }

            /* se va bene il formato chiamata al comando di handler */
            endHandler(current_session, type, master);
            printf("Disconessione da parte del client gestita correttamente"); 
        }
        /* comando non riconosciuto */
        else {
            strcpy(buffer, "Comando inserito non riconosciuto\n");
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Comando non riconosciuto");
        }

    }
    /* se il messaggio è stao inviato con un formato non accettato gli rispondiamo */
    else {

        printf("Invio risposta: formato del comando inserito non valido");
    }
}

/**
 * Funzione per gestire il comando di registrazione
 * @param mex messaggio inviato dall'utente
 * @param int socket del client a cui inviare le comunicazioni
*/
void signupHandler(struct mex message, int socket)
{
    char buffer[DIM_BUFFER];

    /* controllo formato messaggio */
    if(message.opt1 == NULL || message.opt2 == NULL){
        strcpy(buffer, "Compilare correttamente i campi\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito correttamente ma campi non compilati correttamente");
    }
    /* chiamata alla funzione di registrazione*/
    if(signup(message.opt1, message.opt2) == true) {
        strcpy(buffer, "Registrazione avvenuta con successo\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Nuovo utente registrato. Username: %s", message.opt1);
    }
    else {
        strcpy(buffer, "Nome utente già presente\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito correttamente ma utente già presente");
    }
}

/**
 * Funzione per gestire il comando di registrazione
 * @param mex messaggio inviato dall'utente
 * @param int socket del client a cui inviare le comunicazioni
*/
void loginHandler(struct mex message, int socket)
{
    char buffer[DIM_BUFFER];

    /* controllo formato messaggio */
    if(message.opt1 == NULL || message.opt2 == NULL){
        strcpy(buffer, "Compilare correttamente i campi\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito correttamente ma campi non compilati correttamente");
    }

    /* chiamata alla funzione di login*/
    if(login(message.opt1, message.opt2, buffer) == true) {
        strcpy(buffer, "Login avvenuto con successo\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Nuovo utente loggato. Username: %s", message.opt1);
    }
    else {
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito correttamente ma %s", buffer);
    }
}

/**
 * Funzione per gestire l'uscita dal gioco da parte dei client
 * @param session* sessione corrente
 * @param char* tipologia di utente, cambia il tipo di chiusura
*/
void endHandler(struct session* current_session, char* type, fd_set* master)
{
    char buffer[DIM_BUFFER];

    /* nel caso di utente principale va chiusa la connessione ad entrambi i client */
    if(strcmp(type, "MAIN") == 0) {
        /* chiusura del socket principale*/
        close(current_session->sc_main);
        FD_CLR(current_session->sc_main, master);
        logout(current_session->main);

        /* invio della chiusura al secondo client */
        strcpy(buffer, "Il client principale ha deciso di terminare la partita\n");
        send(current_session->sc_secondary, buffer, DIM_BUFFER, 0);   
        close(current_session->sc_secondary);
        FD_CLR(current_session->sc_secondary, master);
        logout(current_session->secondary);

        /* eliminazione fisica della sessione */
        free(current_session);
    }
    /* nel caso di utente principale va chiusa solo la connessione del client secondario */
    else {
        close(current_session->sc_secondary);
        FD_CLR(current_session->sc_secondary, master);
        logout(current_session->secondary);
    }   
}

/**
 * Funzione per che ritorna la lista degli oggetti posseduti.
 * @param message il messaggio inviato
 * @param int il socket del client che ha invito il mex
 * @param session* la sessione in cui ci troviamo
*/
void objsHandler(struct mex message, int socket, struct session* current_session)
{
    int i = 0, max = 0;
    struct object* objs = current_session->set.objs;
    char buffer[DIM_BUFFER];

    /* controllo del formato del messaggio */
    if(message.opt1 != NULL || message.opt2 != NULL){
        strcpy(buffer, "Il comando objs non deve avere parametri\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito correttamente ma campi non compilati correttamente");
    }

    /* per altri scenari aggiugnere righe all'if con else if*/
    if(strcmp(current_session->set.name, "Prison Break") == 0) { /* scenario Prison Break*/
        max = DIM_OBJECTS_PB;
    } 
    /* else if(...) */

    strcpy(buffer, "Oggetti raccolti:\n");
    for (; i < max; i++)
    {
        if(objs[i].pickedUp == true) {
            strcat(buffer, "\t-");
            strcat(buffer, objs[i].name); 
            strcat(buffer, "\n");
        }
    }
    /* dopo aver creato la lista degli oggetti invio al client */
    send(socket, buffer, DIM_BUFFER, 0); 
    printf("Inviati gli oggetti raccolti");
}

/**
 * Funzione per inviare le info riguardanti una partita
 * @param session* sessione di riferimento
 * @param socket client a cui inviare il messaggio
*/
void sendInfos(struct session* current_session, int socket) 
{
    char buffer[DIM_BUFFER];
    /* valorizzaione delle info */
    strcpy(buffer, "SESSION INFOS - Tempo rimanente: ");
    sprintf("%s%d", buffer, remainingTime(current_session->start_time));
    strcpy(buffer, ", Token raccolti: ");
    sprintf("%s%d", buffer, current_session->token_pickedUp);
    strcpy(buffer, ", Token secondari raccolti: ");
    sprintf("%s%d", buffer, current_session->secondary_token_pickedUp);
    strcat(buffer, "\n");

    /* inivio al client */
    send(socket, buffer, DIM_BUFFER, 0); 
}
