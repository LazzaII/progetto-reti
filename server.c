#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/auth.h"
#include "include/structure.h"
#include "include/command.h"
#include "include/utils.h"
#include "scenari/prison_break/prison_break.h"


int main(int argc, char *argv[])
{
    int fdmax, ret, i, listening_socket, communication_socket, player_socket;
    fd_set  master, readfds;
    socklen_t addrlen;
    struct sockaddr_in  sv_addr, cl_addr;
    in_port_t porta = htons(atoi(argv[1]));
    char buffer[DIM_BUFFER];

    /* ------------------------------------
        Avvio e inizializzazione del server 
       ------------------------------------ */ 
    printf("! Avvio e inizializzazione del server in corso...\n\n");

    listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(listening_socket < 0) {
        perror("ERR: Errore nella creazione del socket\n");
        exit(1);
    }

    /* permette il riavvio del server instantanemente senza problemi di porta*/
    ret = setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if(ret < 0) {
        perror("ERR: Errore in setsockopt(...)\n");
        exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = porta;
    sv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listening_socket, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret < 0) {
        perror("ERR: Errore bind\n");
        exit(1);
    }

    ret = listen(listening_socket, 10);
    if(ret < 0) {
        perror("ERR: Errore Listen\n");
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
        memset(buffer, 0, sizeof(buffer));
        readfds = master;
        select(fdmax + 1, &readfds, NULL, NULL, NULL);

        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &readfds)) {
                /* Controllo dello STDIN per ricezione comandi diretti al server */
                if(i == STDIN_FILENO) {
                    scanf("%s", buffer);
                    if(strcmp(buffer, "stop") != 0) {
                        printf("WARN: Comando non riconosciuto.\n\n");
                        continue;
                    }
                    if(gameOn()) {
                        printf("WARN: Impossibile terminare il server, il gioco è in corso.\n\n");
                        continue;
                    }
                    printf("! Chiusura del server...\n\n");
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
                           "******************************************************\n\n", communication_socket);

                    /* invio al client i comandi e scenari disponibili */
                    memset(buffer, 0, sizeof(buffer));
                    setList(buffer);
                    send(i, buffer, DIM_BUFFER, 0); 
                    printf("Invio risposta: scenari disponibili inviati al client.\n\n");

                    FD_SET(communication_socket, &master);
                    if(communication_socket > fdmax) {
                        fdmax = communication_socket;
                    }
                }
                /* Se non è nessuno dei socket precedenti è quello di comunicazione */
                else {
                    char* type = NULL; 
                    ret = recv(i, (void*)buffer, DIM_BUFFER, 0);
                    struct session* current_session = getSession(i, type); /* potrebbe returnane NULL se non trova niente (login, signup)*/

                    /* TODO: chiusura anticipiata client */ 
                    if(ret == 0) {
                        /*printf("Sconnessione socket %d in corso...\n", i);
                        printf("%s", logout_user(i));
                        close(i);
                        printf("Socket %d chiuso.\n", i);
                        FD_CLR(i, &master);
                        printf("Socket %d rimosso dal set dei descrittori.\n"
                               "Sconnessione client effettuata con successo.\n\n", i);
                               
                        sconnetto anche l'altro giocatore se presente
                        if((player_socket = prendi_altro_giocatore(i)) >= 0) {
                            printf("Presente gruppo, disconnessione altro giocatore...");
                            printf("%s", logout_user(player_socket));
                            close(player_socket);
                            printf("Socket %d chiuso.\n", player_socket);
                            FD_CLR(player_socket, &master);
                            printf("Socket %d rimosso dal set dei descrittori.\n"
                                    "Sconnessione client effettuata con successo.\n\n", player_socket);
                            printf("%s", elimina_gruppo());
                        }
                        reset_scenario();
                        printf("Reset dello scenario effettuato.\n\n");
                        continue;*/
                    }

                    printf("******************************************************\n"
                          "Comando: %s\n"
                          "Sessione: %d\n"
                          "Scenario: %s\n"
                          "Utente: %s\n"
                          "Socket: %d\n"
                          "******************************************************\n\n"
                          ,buffer, current_session->id , current_session->set.name, strcmp(type, "MAIN") ? current_session->main->username : current_session->secondary->username, i);
                    commandSwitcher(i, buffer, type, current_session, &master);
                }
            }
        } 
    }
}