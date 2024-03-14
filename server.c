#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/auth.h"
#include "include/structure.h"
#include "include/command.h"
#include "include/utils.h"
#include "scenari/prison_break/prison_break.h"

int main(int argc, char *argv[])
{
    int fdmax, ret, i, listening_socket, communication_socket;
    fd_set  master, readfds;
    socklen_t addrlen;
    struct sockaddr_in  sv_addr, cl_addr;
    in_port_t porta = htons(atoi(argv[1]));
    char buffer[DIM_BUFFER], type[DIM_TYPE];
    struct session* current_session;
    struct user* current_user;


    /* ------------------------------------
        Avvio e inizializzazione del server 
       ------------------------------------ */ 
    printf("! Avvio e inizializzazione del server in corso...\n\n");

    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(listening_socket < 0) {
        perror("ERR: Errore nella creazione del socket - ");
        exit(1);
    }

    /* permette il riavvio del server instantanemente senza problemi di porta*/
    ret = setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if(ret < 0) {
        perror("ERR: Errore in setsockopt(...) - ");
        exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = porta;
    sv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listening_socket, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret < 0) {
        perror("ERR: Errore bind - ");
        exit(1);
    }

    ret = listen(listening_socket, 10);
    if(ret < 0) {
        perror("ERR: Errore Listen - ");
        exit(1);
    }
    printf("OK: Socket di ascolto creato. fd: %d\n\n", listening_socket);

    /* Azzeramento e inizializzaizione dei set (princpale e di appoggio) */
    FD_ZERO(&master);
    FD_ZERO(&readfds);

    FD_SET(listening_socket, &master);
    FD_SET(STDIN_FILENO, &master);
    fdmax = listening_socket;

    printf("******************* SERVER AVVIATO *******************\n"
           "Comandi disponibili:\n"
           "\tstop\ttermina il server\n"
           "******************************************************\n\n");
    
    for(;;) 
    {
        memset(buffer, 0, DIM_BUFFER);
        readfds = master;
        select(fdmax + 1, &readfds, NULL, NULL, NULL);

        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &readfds)) {
                /* Controllo dello STDIN per ricezione comandi diretti al server */
                if(i == STDIN_FILENO) {
                    scanf("%s", buffer);
                    printf("\n******************************************************\n");
                    if(strcmp(buffer, "stop") != 0) {
                        printf("WARN: Comando non riconosciuto.\n"
                            "******************************************************\n");
                        continue;
                    }
                    printf("! Chiusura del server...\n\n");
                    if(gameOn() == true) { /* se è presente una partita non termino */
                        printf("WARN: Impossibile terminare il server, il gioco è in corso.\n"
                                "******************************************************\n");
                        continue;
                    }
                    deleteUsers(); /* basta eliminare gli utenti perchè se siamo arrivati qui le sessioni sono già state deallocate */
                    close(listening_socket);
                    printf("OK: Server chiuso correttamente\n"
                            "******************************************************\n");
                    exit(0);
                }
                /* Controllo il socket di ascolto, addetto alle nuove connessioni*/
                else if(i == listening_socket) {
                    addrlen = sizeof(cl_addr);
                    memset(&cl_addr, 0, sizeof(cl_addr));
                    communication_socket = accept(listening_socket, (struct sockaddr*)&cl_addr, &addrlen);
                    
                    if(communication_socket < 0) {
                        perror("ERR: Errore accept");
                        exit(1);
                    }
                    printf("******************************************************\n"
                           "Nuovo client connesso, socket: %d\n"
                           , communication_socket);

                    /* invio al client gli scenari disponibili */
                    memset(buffer, 0, DIM_BUFFER);
                    instruction(buffer);
                    send(communication_socket, buffer, DIM_BUFFER, 0); 
                    printf("Risposta server: scenari disponibili inviati al client.\n"
                           "******************************************************\n\n");
                    
                    FD_SET(communication_socket, &master);
                    if(communication_socket > fdmax) {
                        fdmax = communication_socket;
                    }
                }
                /* Se non è nessuno dei socket precedenti è quello di comunicazione */
                else {
                    memset(buffer, 0, DIM_BUFFER);
                    ret = recv(i, (void*)buffer, DIM_BUFFER, 0);
                    current_session = getSession(i, type);
                    /* potrebbe returnane NULL se non trova niente (qualsiasi cosa prima di aver fatto comando start)*/

                    if(ret == 0) {
                        /* se il client ancora non è entrato in nessuna sessione */
                        if(current_session == NULL) {
                            printf("******************************************************\n"
                              "! Sconnessione socket %d in corso...\n", i);
                            close(i);
                            printf("Socket %d chiuso.\n"
                                    "Il socket non faceva parte di nessuna partita.\n", i);
                            printf("SUCCES: Disconessione eseguita con successo\n"
                                "******************************************************\n\n");
                            continue;
                        }

                        /* 
                            se invece il client faceva parte di una sessione si disconnette il socket e 
                            si controlla il tipo di utente in modo da gestire la chiusura aggiuntiva del socket secondario.
                        */
                        current_user = strcmp(type, "MAIN") == 0 ? current_session->main : current_session->secondary;
                        printf("******************************************************\n"
                              "! Sconnessione socket %d in corso...\n"
                              "! Eliminazione utente: %s", i, current_user->username);
                        logout(current_user);
                        close(i);
                        printf("Socket %d chiuso.\n", i);
                        FD_CLR(i, &master);
                        printf("Socket %d rimosso dal set dei descrittori.\n"
                                "OK: Disconnessione client eseguita con successo.\n\n", i);
                               
                        /* se il client è principale va disconnesso anche il client secondario*/
                        if(strcmp(type, "MAIN") == 0) {
                            printf("Disconessione altro giocatore..."
                                    "! Sconnessione socket %d in corso...\n"
                                    "! Eliminazione utente: %s", current_session->secondary->socket, current_session->secondary->username);
                            close(current_session->secondary->socket);
                            printf("OK: Socket %d chiuso.\n", current_session->secondary->socket);
                            FD_CLR(current_session->secondary->socket, &master);
                            printf("OK: Socket %d rimosso dal set dei descrittori.\n"
                                    "OK: Disconnessione client secondario eseguita con successo.\n\n", current_session->secondary->socket);
                            printf("! Eliminazione della sessione");
                            free(current_session); /* è già stata rimossa dalla lista nel login, manca solo da liberare lo spazio*/
                            printf("OK: Sessione eliminata con successo");
                        }
                        printf("SUCCES: Disconessione eseguita con successo\n"
                                "******************************************************\n\n");
                        continue;
                    }
                    
                    printf("******************************************************\n"
                          "Comando: %s\n"
                          "Sessione: %d\n"
                          "Scenario: %s\n"
                          "Utente: %s\n"
                          "Socket: %d\n"
                          "Risposta server: "
                          ,buffer, 
                          current_session ? current_session->id : -1,
                          current_session ? current_session->set.name : "non in sessione", 
                          current_session ? (strcmp(type, "MAIN") == 0 ? current_session->main->username : current_session->secondary->username) : "non in sessione", 
                          i); 
                    commandSwitcher(i, buffer, type, current_session, &master);
                    printf("\n******************************************************\n\n");
                }
            }
        } 
    }
}