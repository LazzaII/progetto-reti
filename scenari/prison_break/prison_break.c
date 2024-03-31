#include "prison_break.h"

/* INIZIALIZZAZIONE DELLE STRUTTURE */
struct riddle riddles[DIM_RIDDLES_PB] = {
    {
        "Vediamo se sei stato attento. Sono di andata e di ritorno, chi sono? Tempo di...\n",
        "Anna", /* soluzione */
        0,
        "Costruire una bomba è molto facile, hai provato con il t----on- e il s--o--?\n"
    },
    {
        "Il codice del lucchetto è il continuo di questa sequenza.\n"
        "11\n"
        "21\n"
        "1211\n"
        "111221\n",
        "312211", /* soluzione */
        0,
        "Dentro la scatola c'è del **sapone** e altre **monete3**.\n"
    },
};

struct object objects[DIM_OBJECTS_PB] = {
    { /* 0 */
        "McLovin",
        "McLovin è un ex chimico, forse può esserti utile.\n",
        "Vuoi provare a scappare? Va bene, ti aiuterò solo a patto di rispondermi ad un indovinello.\n",
        0,
        0,
        1, 
        0,
        0,
        &riddles[0]
    },
    { /* 1 */
        "telefono",
        "Sul numero di telefono è salvato un solo numero.\n",
        "Chiamo il numero salvato...\nBiiiip. Biiiip.\nNon risponde nessuno\n",
        0,
        0,
        0, 
        1,
        0,
        NULL
    },
    { /* 2 */
        "monete1",
        "Sono delle monete d'oro.\n",
        "É uscita testa.\n",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 3 */
        "scatola",
        "La scatola sembra chiusa.\n",
        "É bloccata da un lucchetto a sono 6 cifre.\n",
        0,
        0,
        0, 
        0,
        0,
        &riddles[1]
    },
    { /* 4 */
        "monete2",
        "Sono delle monete d'oro.\n",
        "É uscita croce.\n",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 5 */
        "sapone",
        "Bagnodoccia Vidal, tutto in uno!\n",
        "Ancora non è il momento di lavarsi.\n",
        0,
        0,
        0, 
        1,
        0,
        NULL
    },
    { /* 6 */
        "monete3",
        "Sono delle monete d'oro.\n",
        "É uscita croce.\n",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 7 */
        "sbarre",
        "Non sembrano così resistenti...\n",
        "Non sono resistenti ma provare ad aprirle a mano è troppo forse.\n",
        0,
        0,
        0, 
        0,
        0,
        NULL
    },
    { /* 8 */
        "bomba",
        "Hai costruito **bomba**. Guarda cosa si può fare con del semplice sapone e delle batterie.\n",
        "Sei folle?! Così ci farai saltare tutti in aria!\n",
        0,
        0,
        0, 
        1,
        0,
        NULL
    }
};

struct location locations[DIM_LOCATIONS_PB] = {
    {
        "letto",
        "Sotto il letto hai trovato un **telefono** e delle **monete1**",
        {&objects[1], &objects[2]}
    },
    {
        "scaffale",
        "Nello scaffale c'è una **scatola** e delle **monete2**.",
        {&objects[3], &objects[4]}
    },
    {
        "finestra",
        "la finestra ha le **sbarre**.",
        {&objects[7]}
    }
};

struct set prison_break = {
    1,
    "Prison Break",
    "Sei in prigione. Ti trovi sul ++letto++, davanti a te c'è uno ++scaffale++ e una ++finestra++. Mentre dall'altro lato della stanza c'è il tuo compagno di stanza **McLovin**",
    locations,
    objects,
};

/**
 * Funzione specifica pe il comando look dello scenario Prison Break
 * @param message messaggio inviato dall'utente
 * @param socket socket a cui inviare le comunicazioni
 * @param session* sessione corrente
*/
void lookHandlerPB(struct mex message, int socket, struct session* current_session)
{
    int i, j;
    char buffer[DIM_BUFFER];
    
    memset(buffer, 0, DIM_BUFFER);

    /* controllo se il messaggio è nel formato corretto */
    if(message.opt2 == NULL) {
        if(message.opt1 == NULL) {
            strcpy(buffer, current_session->set.description);
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Comando look sullo scenario");
        }
        else {
            /* controllo delle location */
            for (i = 0; i < DIM_LOCATIONS_PB; i++)
            {
                if(strcmp(locations[i].name, message.opt1) == 0) {
                    /* se la location è corretta si invia la risposta e si scoprono gli oggetti */
                    strcpy(buffer, locations[i].description);
                    send(socket, buffer, DIM_BUFFER, 0); 
                    printf("Comando look sulla location");

                    /* sblocco degli oggetti data la location*/
                    for (j = 0; j < MAX_OBJ_LOC; j++)
                    {
                        if(current_session->set.locations[i].objs[j])
                            current_session->set.locations[i].objs[j]->found = true;
                    }

                    return;
                }
            }
            
            /* controllo degli oggetti */
            for (i = 0; i < DIM_OBJECTS_PB; i++)
            {
                if(strcmp(objects[i].name, message.opt1) == 0 && objects[i].found == true) {
                    /* se l'oggetto è corretto si invia la risposta e si scoprono gli oggetti */
                    strcpy(buffer, objects[i].description);
                    send(socket, buffer, DIM_BUFFER, 0); 
                    printf("Comando look sugli oggetti");
                    
                    return;
                }

            }

            /* oggetto o location inseriti non validi*/
            strcpy(buffer, "L'oggetto o la location inserita non è valida");
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Comando look su oggetto/location non valido");
        }
    }
    /* formato per il comando look non valido */
    else {
        strcpy(buffer, "Formato per look non valido: look [loc | obj]: senza argomenti rende la descrizione dello scenario, altrimenti la descrizione dell'oggetto/location in questione.\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Formato del comando look non valido");
    }
} 

/**
 * Funzione specifica pe il comando take dello scenario Prison Break
 * @param message messaggio inviato dall'utente
 * @param socket socket a cui inviare le comunicazioni
 * @param session* sessione corrente
*/
void takeHandlerPB(struct mex message, int socket, struct session* current_session)
{
    int i;
    char buffer[DIM_BUFFER];
    
    memset(buffer, 0, DIM_BUFFER);

    /* controllo se il messaggio è nel formato corretto */
    if(message.opt1 != NULL && message.opt2 == NULL) {
        /* controllo su McLovin */
        if(strcmp(message.opt1, "McLovin") == 0 || strcmp(message.opt1, "Sbarre") == 0 ) {
            strcpy(buffer, "Non è possibile fare take su questo oggetto\n");
            send(socket, buffer, DIM_BUFFER, 0); 
            printf("Comando take su oggetto non valido");
            return;
        }

        /* controllo degli oggetti */
        for (i = 0; i < DIM_OBJECTS_PB; i++)
        {
            if(strcmp(objects[i].name, message.opt1) == 0 && current_session->set.objs[i].found == true) {
                if(current_session->set.objs[i].pickedUp == true) {
                    strcpy(buffer, "Oggetto già raccolto\n");
                }
                /* se l'oggetto è corretto si invia la risposta e si prende l'oggetto oggetti */
                else if(objects[i].is_token == true) {
                    strcpy(buffer, "Oggetto raccolto, era pure un token\n");
                    current_session->token_pickedUp++;
                } 
                else if(objects[i].is_secondary_token == true) {
                    strcpy(buffer, "Oggetto raccolto, era pure un token secondario\n");
                    current_session->secondary_token_pickedUp++;
                }
                else 
                    strcpy(buffer, "Oggetto raccolto\n");

                current_session->set.objs[i].pickedUp = true;

                send(socket, buffer, DIM_BUFFER, 0); 
                printf("Comando take sugli oggetti");
                return;
            }
        }
        strcpy(buffer, "Take su oggetto non valido/non scoperto\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Comando take su oggetto non valido/non scoperto");
        return;
    }
    /* formato per il comando take non valido */
    else {
        strcpy(buffer, "Formato per take non valido: take obj\n");
        send(socket, buffer, DIM_BUFFER, 0); 
        printf("Formato del comando look non valido");
    }
}

/**
 * Funzione specifica pe il comando use dello scenario Prison Break
 * @param message messaggio inviato dall'utente
 * @param socket socket a cui inviare le comunicazioni
 * @param session* sessione corrente
 * @return booleano per controllare la vittoria
*/
bool useHandlerPB(struct mex message, int socket, struct session* current_session)
{
    char buffer[DIM_BUFFER];
    bool win = false;
    
    memset(buffer, 0, DIM_BUFFER);

    if(message.opt1 == NULL){
        strcpy(buffer, "Comando use usato in maniera non valida, potrebbe essere dovuto al formato sbagliato oppure ad aver utilizzato il comando con oggetti non raccolti/scoperti\n");
    }
    /* switch degli use, ad ogni comando è associato qualcosa di diverso con diverse sfumature*/
    else if(strcmp(message.opt1, "McLovin") == 0 && current_session->set.objs[0].found) {
        /* attivo il quiz se non è già stato attivato */
        if(message.opt2 == NULL) {
            if(current_session->set.objs[0].riddle->solved == false) {
                strcpy(buffer, objects[0].use_description);
                send(socket, buffer, DIM_BUFFER, 0); 
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, objects[0].riddle->description);
                current_session->active_riddle = true;
                current_session->pos_riddle = 0;
            }
            else strcpy(buffer, "Quiz già risolto\n");
        }
        else strcpy(buffer, "Comando use su McLovin usato in maniera errata\n");
    }
    else if(strcmp(message.opt1, "telefono") == 0 && current_session->set.objs[1].pickedUp == true) {
        /* si può creare la bomba telefono sapone */
        if(message.opt2 && strcmp(message.opt2, "sapone") == 0) {
            strcpy(buffer, objects[8].description);
            current_session->set.objs[8].found = true;
        }
        /* se il giocatore secondario è connesso si fa la chiamata */
        else if (current_session->secondary)
        {   
            printf("Comunicazione con il secondo giocatore - ");
            strcpy(buffer, "chiamata in corso");
            send(current_session->secondary->socket, buffer, DIM_BUFFER, 0); 
            memset(buffer, 0, DIM_BUFFER);
            strcpy(buffer, "*** Chiamata in corso al secondino corrotto ***\n");
            current_session->active_call = true;
        }
        else strcpy(buffer, objects[1].use_description);

    }
    else if(strcmp(message.opt1, "monete1") == 0 && current_session->set.objs[2].pickedUp == true) {
        strcpy(buffer, objects[2].use_description);
    }
    else if(strcmp(message.opt1, "scatola") == 0 && current_session->set.objs[3].pickedUp == true) {
        /* attivo il quiz se non è già stato attivato */
        if(message.opt2 == NULL) {
            if(current_session->set.objs[3].riddle->solved == false) {
                strcpy(buffer, objects[0].use_description);
                send(socket, buffer, DIM_BUFFER, 0); 
                memset(buffer, 0, DIM_BUFFER);
                strcpy(buffer, objects[3].riddle->description);
                current_session->active_riddle = true;
                current_session->pos_riddle = 1;
            }
            else strcpy(buffer, "Quiz già risolto\n");
        }
        else strcpy(buffer, "Comando use su McLovin usato in maniera errata\n");
    }
    else if(strcmp(message.opt1, "monete2") == 0 && current_session->set.objs[4].pickedUp == true) {
        strcpy(buffer, objects[4].use_description);
    }
    else if(strcmp(message.opt1, "sapone") == 0 && current_session->set.objs[5].pickedUp == true) {
        /* si può creare la bomba sapone telefono */
        if(message.opt2 && strcmp(message.opt2, "telefono") == 0) {
            strcpy(buffer, objects[8].description);
            objects[8].found = true;
        }
        else strcpy(buffer, objects[5].use_description);
    }
    else if(strcmp(message.opt1, "monete3") == 0 && current_session->set.objs[6].pickedUp == true) {
        strcpy(buffer, objects[6].use_description);
    }
    /* coi prossimi due oggetti si controlla se si è vinto*/
    else if(strcmp(message.opt1, "sbarre") == 0 && current_session->set.objs[7].found == true) {
        if(message.opt2 && strcmp(message.opt2, "bomba") == 0) {
            strcpy(buffer, "$$$ Complimenti sei riuscito ad evadere! Hai completato l'escape room! $$$\n");
            win = true;
        }
        else strcpy(buffer, objects[7].use_description);
    }
    else if(strcmp(message.opt1, "bomba") == 0 && current_session->set.objs[8].pickedUp == true) {
        if(message.opt2 && strcmp(message.opt2, "sbarre") == 0) {
            strcpy(buffer, "$$$ Complimenti sei riuscito ad evadere! Hai completato l'escape room! $$$\n");
            win = true;
        }
        else strcpy(buffer, objects[8].use_description);
    }
    else {
        strcpy(buffer, "Comando use usato in maniera non valida, potrebbe essere dovuto al formato sbagliato oppure ad aver utilizzato il comando con oggetti non raccolti/scoperti\n");
    }
    send(socket, buffer, DIM_BUFFER, 0); 
    printf("Usato comando use");
    return win;
}

/**
 * Funzione specifica pe il comando use dello scenario Prison Break
 * @param message risposta inviata
 * @param session* sessione corrente
*/
void riddleHandlerPB(struct mex message, struct session* current_session)
{
    char buffer[DIM_BUFFER];
    int pos = current_session->pos_riddle;
    
    memset(buffer, 0, DIM_BUFFER);

    if(strcmp(riddles[pos].solution, message.command) == 0) {
        strcpy(buffer, "Risposta corretta\n");
        send(current_session->main->socket, buffer, DIM_BUFFER, 0); 
        memset(buffer, 0, DIM_BUFFER);
        strcpy(buffer, riddles[pos].mex_sol);

        /* vanno attivati gli oggetti */
        if(pos == 1) {
            current_session->set.objs[5].found = true;
            current_session->set.objs[6].found = true;
        }

        riddles[pos].solved = true;
        current_session->active_riddle = false;
    }
    else {
        strcpy(buffer, "Risposta errata, ritenta\n");
    }

    send(current_session->main->socket, buffer, DIM_BUFFER, 0); 
    printf("Risposta al quiz");
}

/**
 * Funzione specifica pe il comando use dello scenario Prison Break
 * @param message risposta inviata
 * @param session* sessione corrente
 * @param char* tipo di utente
 * @param fd_set*
*/
void callHandlerPB(struct mex message, struct session* current_session, char* type, fd_set* master)
{
    char buffer[DIM_BUFFER];

    memset(buffer, 0, DIM_BUFFER);

    /* giocatore princiaple */
    if(strcmp("MAIN", type) == 0) { 
        strcpy(buffer, "*** In attesa di risposta dal client secondario ***\n");
        send(current_session->main->socket, buffer, DIM_BUFFER, 0); 
        printf("Prigioniero continua a chiamare il secondino");
    }
    /* giocatore secondario */
    else {
        /* TODO non chiude bene la sessione*/
        printf("Risposta del secondino");
        /* controllo risposta del secondino che deve essere tra 1 e 3*/
        if(atoi(message.command) > 4 || atoi(message.command) < 0) {
            printf("Risposta del secondino non valida");
            strcpy(buffer, "Richiesta errata, puoi richiedere solamente tra 1 e 3 monete [compresi]\n");
            send(current_session->secondary->socket, buffer, DIM_BUFFER, 0); 
        }  
        /* se la richiesta di soldi è maggiore o uguale*/
        else if(atoi(message.command) <= current_session->secondary_token_pickedUp) {
            printf("\n *** ESCAPE ROOM FINITA - chiusura della sessione di gioco ***");
            strcpy(buffer, "$$$ Complimenti hai vinto! Avevi abbastanza denaro per scappare, il secondino ti ha fatto evadere. $$$\n");
            send(current_session->main->socket, buffer, DIM_BUFFER, 0); 
            memset(buffer, 0, DIM_BUFFER);
            strcpy(buffer, "$$$ Il prigioniero aveva abbastanza denaro quindi l'hai fatto evadere $$$\n");
            send(current_session->secondary->socket, buffer, DIM_BUFFER, 0); 

            /* chiusura della partita */
            close(current_session->main->socket);
            FD_CLR(current_session->main->socket, master);
            logout(current_session->main);
  
            close(current_session->secondary->socket);
            FD_CLR(current_session->secondary->socket, master);
            logout(current_session->secondary);

            deleteSession(current_session->id);
        } 
        else {
            sprintf(buffer, "La risposta del secondino è %s, però te hai solo %d\nContinua a cercare monete oppure prova adevadere in autonomia."
                    , message.command, current_session->secondary_token_pickedUp);
            send(current_session->main->socket, buffer, DIM_BUFFER, 0); 
            memset(buffer, 0, DIM_BUFFER);
            strcpy(buffer, "*** Monete insufficienti da parte del prigioniero, in attesa di nuova chiamata... ***\n");
            send(current_session->secondary->socket, buffer, DIM_BUFFER, 0); 
            printf(" - Il prigioniero non aveva abbastanza denaro");
            current_session->active_call = false;
        }
    }

}
