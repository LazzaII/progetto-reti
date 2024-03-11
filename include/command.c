#include "command.h"

/**
 * Funzione per valorizzare il buffer con le istruzioni
 * @param char* buffer da valorizzare
*/
void instruction(char* buffer)
{
    strcpy(buffer, 
            "Benvenuto in questa escape room, per prima cosa è necessario registrarsi. Se si ha già un account si può fare il login.\n"
            "\t- signup username pwd: comando per registrarsi\n"
            "\t- login username pwd: comando per effettuare il login\n\n"
            "Altrimenti per uscire puoi usare il comando:\n"
            "\t- end");
}

/**
 * Funzione per valorizzare buffer con la lista degli scenari
 * @param char* buffer da valorizzare
*/
void setList(char* buffer)
{
    strcpy(buffer, 
            "Lista degli scenari disponibili:\n"
            "\t- 1: Prison Break\n"
            "Tipo di personaggi disponibili:\n"
            "\t- 1: Principale\n"
            "\t- 2: Secondario\n"
            "Usare il comando start num tipo per avviare lo scenario\n");
}

/**
 * Funzione per valorizzare buffer con la lista dei comandi
 * @param char* buffer da valorizzare
*/
void commandList(char* buffer) 
{
    strcpy(buffer, 
            "Lista dei comandi disponibili:\n"
            "\t- look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggetto/location in questione.\n"
            "\t- take obj: raccoglie l'oggetto <obj> indicato come parametro.\n"
            "\t- use obj1 [obj2]: permette di utilizzare l'oggetto <obj> indicato come parametro o di usarlo in maniera combinata con l'oggetto <obj2>.\n"
            "\t- objs: rende la lista degli oggetti raccolti.\n"
            "\t- end: termina la partita ed esce dal gioco.\n");
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

    memset(buffer, 0, DIM_BUFFER);

    /* TODO check fine tempo */

    /* TODO check riddle */

    /* TODO check chiamata */

    /* Switch dei comandi con relativa chiamata ai vari handler */
    if(substringed_mex.ok == true) {
        /* comandi di autenticazione */
        if(strcmp(substringed_mex.command, "signup") == 0) {
            /* se l'utente ha già fatto login non si può usare il comando signup */
            if(userLogged(socket) == true) {
                strcpy(buffer, "Login già eseguito, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Login già eseguito, comando non disponibile");
            }
            else /* chiamata all'handler */
                signupHandler(substringed_mex, socket);
        }
        else if (strcmp(substringed_mex.command, "login") == 0) {
            /* se l'utente ha già fatto login non si può usare il comando login */
            if(userLogged(socket) == true) {
                strcpy(buffer, "Login già eseguito, comando non disponibile\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Login già eseguito, comando non disponibile");
            } 
            else /* chiamata all'handler */
                loginHandler(substringed_mex, socket);
        }
        /* comandi di inizio gioco*/
        else if (strcmp(substringed_mex.command, "start") == 0) {
            /* si controlla se l'utente ha già fatto login, se lo ha fatto si controlla che non sia già in partita*/
            if(userLogged(socket) == true) {
                if((strcmp(type, "MAIN") == 0 && current_session->main->inGame) || ((strcmp(type, "SEC") == 0) && current_session->secondary->inGame)) {
                    strcpy(buffer, "Comando start non disponibile dopo aver già fatto start\n");
                    send(socket, buffer, DIM_BUFFER, 0); 
                    printf("Tentato comando start a partita iniziata");
                }
                else 
                    startHandler(substringed_mex, socket);
            }
            else {
                strcpy(buffer, "Per usare questo comando devi prima fare login, altrimenti se non hai un account prima devi fare signup\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Tentato comando senza login");
            }
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
                strcpy(buffer, "Formato non ricosciuto per il comando end: end\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando inserito riconosciuto ma attributi non riconosciuti");
                return;
            }

            /* si controlla se esce prima di aver joinato una partita */
            if(current_session == NULL) {
                close(socket);
                FD_CLR(socket, master);
            }
            else
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
        strcpy(buffer, "Formato del comando inviato non riconosciuto\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Formato del comando inserito non valido");
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

    memset(buffer, 0, DIM_BUFFER);

    /* controllo formato messaggio */
    if(message.opt1 == NULL || message.opt2 == NULL){
        strcpy(buffer, "Compilare correttamente i campi: signup username pwd\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito riconosciuto ma campi non compilati correttamente");
        return;
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

    memset(buffer, 0, DIM_BUFFER);

    /* controllo formato messaggio */
    if(message.opt1 == NULL || message.opt2 == NULL){
        strcpy(buffer, "Compilare correttamente i campi: login username pwd\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito riconosciuto ma campi non compilati correttamente");
        return;
    }


    /* chiamata alla funzione di login*/
    if(login(message.opt1, message.opt2, socket, buffer) == true) {
        strcpy(buffer, "Login avvenuto con successo\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Nuovo utente loggato. Username: %s", message.opt1);

        memset(buffer, 0, DIM_BUFFER);
        setList(buffer);
        send(socket, buffer, DIM_BUFFER, 0); 
        printf(" - Invio lista scenari.");
    }
    else {
        send(socket, buffer, DIM_BUFFER, 0); 
        strtok(buffer, "\n"); /* lo stampa già il server, ma il client no*/
        printf("Comando inserito riconosciuto ma %s", buffer);
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

    memset(buffer, 0, DIM_BUFFER);

    /* nel caso di utente principale va chiusa la connessione ad entrambi i client */
    if(strcmp(type, "MAIN") == 0) {
        /* chiusura del socket principale*/
        close(current_session->main->socket);
        FD_CLR(current_session->main->socket, master);
        logout(current_session->main);

        /* invio della chiusura al secondo client */
        strcpy(buffer, "Il client principale ha deciso di terminare la partita\n");
        send(current_session->secondary->socket, buffer, DIM_BUFFER, 0);   
        close(current_session->secondary->socket);
        FD_CLR(current_session->secondary->socket, master);
        logout(current_session->secondary);

        /* eliminazione fisica della sessione */
        free(current_session);
    }
    /* nel caso di utente principale va chiusa solo la connessione del client secondario */
    else {
        close(current_session->secondary->socket);
        FD_CLR(current_session->secondary->socket, master);
        logout(current_session->secondary);
    }   
}

/**
 * Funzione per inziare una nuova partita 
 * @param message messaggio inviato dal client
 * @param socket il socket del client che ha invito il mex
*/
void startHandler(struct mex message, int socket)
{
    char buffer[DIM_BUFFER];
    struct session* s;

    memset(buffer, 0, DIM_BUFFER);

    /* si controlla il formato del messaggio */
    if(message.opt1 == NULL || message.opt2 == NULL){
        strcpy(buffer, "Compilare correttamente i campi: start num tipo\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito riconosciuto ma campi non compilati correttamente");
        return;
    }

    /* si controlla che num sia uno di quelli validi */
    if(strcmp(message.opt1, "1") == 0) { /* 1 = Prison Break*/
        /* Giocatore principale -> si crea una nuova sessione*/
        if(strcmp(message.opt2, "1") == 0) { 
            createSession(socket, 1);
            commandList(buffer);
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Scenario Prison Break inizializzato, inviati comandi al giocatore principale");
        }
        /* Giocatore secondario -> si fa joinare nella prima sessione libera e si invia la comunicazione al giocatore principale che è entrato un nuovo giocatore*/
        else if(strcmp(message.opt2, "2") == 0) {
            s = firstFreeSession(1); /* 1 sta per Prison Break */
            if(s) {
                s->secondary = findUserFromSocket(socket);
                s->secondary->inGame = true; /* si mette in game il giocatore */
                strcpy(buffer, "Il giocatore secondario è un secondino corrotto in attesa della chiamata del prigioniero.\n"
                                "Potrebbe non essere mai chiamato in causa, la sua presenza non è necessaria per vincere.\n"
                                "Nel momento in cui arriva la chiamata il secondino può decidere quanto monete richiedere al prigioniero per farlo evadere (tra 1 e 3).\n"
                                "Fino a quel momento non può fare altro.");
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Scenario Prison Break joinato, inviate indicazioni al giocatore secondario");
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, "*** Secondino disponbile... shhh ***");
                send(s->main->socket, buffer, DIM_BUFFER, 0);
                printf("Comunicazione di join inviata al personaggio principale");
            }
            else { /* se non ci sono sessioni libere rimandiamo le istruzioni e ci aspettiamo una nuova start */
                strcpy(buffer, "!ALERT!: Non ci sono sessioni libere al momento\n");
                send(socket, buffer, DIM_BUFFER, 0); 
                memset(buffer, 0, DIM_BUFFER);
                setList(buffer);
                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Impossibile far joinare un giocatore secondario non ci sono sessioni libere, rimandate le istruzioni iniziali");
            }
        }
        else {
            strcpy(buffer, "Compilare correttamente i campi del comando start: tipo di giocatore inserito non esistente\n");
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Comando inserito riconosciuto ma campi non compilati correttamente");
        }
    }
    /* else if(..) per aggiungere altri scenari */
    else {
        strcpy(buffer, "Compilare correttamente i campi del comando start: scenario non esistente \n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito riconosciuto ma campi non compilati correttamente");
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

    memset(buffer, 0, DIM_BUFFER);

    /* controllo del formato del messaggio */
    if(message.opt1 != NULL || message.opt2 != NULL){
        strcpy(buffer, "Il comando objs non deve avere parametri\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando inserito riconosciuto ma campi non compilati correttamente");
        return;
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

    memset(buffer, 0, DIM_BUFFER);
    /* valorizzaione delle info */
    strcpy(buffer, "SESSION INFOS - Tempo rimanente: ");
    sprintf("%s%d", buffer, remainingTime(current_session->start_time));
    strcpy(buffer, "secondi , Token raccolti: ");
    sprintf("%s%d", buffer, current_session->token_pickedUp);
    strcpy(buffer, ", Token secondari raccolti: ");
    sprintf("%s%d", buffer, current_session->secondary_token_pickedUp);
    strcat(buffer, "\n");

    /* inivio al client */
    send(socket, buffer, DIM_BUFFER, 0); 
}
