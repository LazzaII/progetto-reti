#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include "include/auth.h"
#include "include/structure.h"
#include "include/command.h"
#include "include/utils.h"
#include "scenari/prison_break/prison_break.h"

#define DIM_BUFFER 1024

int main(int argc, char *argv[])
{
    int nSocket, fdMax, ret, i, listeningSocket, comm_sock, sock_giocatore;
    fd_set  master, readFds;
    socklen_t addrlen;
    struct sockaddr_in  sv_addr, cl_addr;
    in_port_t porta = htons(atoi(argv[1]));
    char buffer[DIM_BUFFER];

    /* ------------------------------------
        Avvio e inizializzazione del server 
       ------------------------------------ */ 
    printf("! Avvio e inizializzazione del server in corso...\n");

    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listeningSocket < 0) {
        perror("ERR: Errore nella creazione del socket");
        exit(1);
    }

    /* permette il riavvio del server instantanemente senza problemi di porta*/
    ret = setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if(ret < 0) {
        perror("ERR: Errore in setsockopt(...)");
        exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = porta;
    sv_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(listeningSocket, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret < 0) {
        perror("ERR: Errore bind");
        exit(1);
    }

    ret = listen(listeningSocket, 10);
    if(ret < 0) {
        perror("ERR: Errore Listen");
        exit(1);
    }
    printf("OK: Socket di ascolto creato. fd: %d\n", listeningSocket);

    FD_ZERO(&master);
    FD_ZERO(&readFds);

    FD_SET(listeningSocket, &master);
    FD_SET(STDIN_FILENO, &master);
    fdMax = listeningSocket;

    printf("******************* SERVER AVVIATO *******************\n"
           "Comandi disponibili:\n"
           "\tstop\ttermina il server\n"
           "******************************************************\n\n");
    
    for(;;) {

    }

}