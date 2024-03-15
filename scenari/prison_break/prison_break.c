#include "prison_break.h"

/* INIZIALIZZAZIONE DELLE STRUTTURE */
struct riddle riddles[DIM_RIDDLES_PB] = {
    {
        "Vediamo se sei stato attento. Sono di andata e di ritorno, chi sono?",
        "Tempo di Anna", /* soluzione */
        0,
        "Costruire una bomba è molto facile, hai provato con il t----on- e il s--o--?"
    },
    {
        "Il codice del lucchetto è il continuo di questa sequenza.\n"
        "11\n"
        "21\n"
        "1211\n"
        "111221\n",
        "312211", /* soluzione */
        0,
        "Dentro la scatola c'è del **sapone** e altre **monete3**."
    },
};

struct object objects[DIM_OBJECTS_PB] = {
    { /* 0 */
        "McLovin",
        "McLovin è un ex chimico, forse può esserti utile.",
        "Vuoi provare a scappare? Va bene, ti aiuterò solo a patto di rispondermi ad un indovinello.",
        0,
        0,
        1, 
        0,
        0,
        &riddles[0]
    },
    { /* 1 */
        "telefono",
        "Sul numero di telefono è salvato un solo numero.",
        "Chiamo il numero salvato. Biiiip. Biiiip.",
        0,
        0,
        0, 
        1,
        0,
        NULL
    },
    { /* 2 */
        "monete1",
        "Sono delle monete d'oro.",
        "É uscita testa.",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 3 */
        "scatola",
        "La scatola sembra chiusa.",
        "É bloccata da un lucchetto a sono 6 cifre.",
        0,
        0,
        0, 
        0,
        0,
        &riddles[1]
    },
    { /* 4 */
        "monete2",
        "Sono delle monete d'oro.",
        "É uscita croce.",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 5 */
        "sapone",
        "Bagnodoccia Vidal, tutto in uno!",
        "Ancora non è il momento di lavarsi.",
        0,
        0,
        0, 
        1,
        0,
        NULL
    },
    { /* 6 */
        "monete3",
        "Sono delle monete d'oro.",
        "É uscita croce.",
        0,
        0,
        0, 
        0,
        1,
        NULL
    },
    { /* 7 */
        "sbarre",
        "Non sembrano così resistenti...",
        "Non sono resistenti ma provare ad aprirle a mano è troppo forse.",
        0,
        0,
        0, 
        0,
        0,
        NULL
    },
    { /* 8 */
        "bomba",
        "Guarda cosa si può fare con del semplice sapone e delle batterie.",
        "Sei folle?! Così ci farai saltare tutti in aria!",
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

void takeHandlerPB(struct mex message, int socket, struct session* current_session)
{

}

void useHandlerPB(struct mex message, int socket, struct session* current_session)
{

}

