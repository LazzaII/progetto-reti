
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include "include/structure.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4242
#define DIM_BUFFER 1024

int main(int argc, char *argv[])
{
    int fdmax, ret, i;
    fd_set  master, read_fds;
    char buffer[DIM_BUFFER];
    struct sockaddr_in sv_addr;
    in_port_t porta = htons(SERVER_PORT);

    char choosenSet[2];
    /* per bloccare l'invio dei mex in caso di giocatore secondario */
    bool canSend = true;

    printf("! Avvio e inizializzazione del client in corso...\n\n");
    fdmax = socket(AF_INET, SOCK_STREAM, 0);
    if(fdmax < 0) {
        perror("ERR: Errore creazione socket");
        exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = porta;
    inet_pton(AF_INET, SERVER_IP, &sv_addr.sin_addr);

    ret = connect(fdmax, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret < 0) {
        perror("ERR: Errore nella connessione con il server");
        exit(1);
    }
    printf("OK: Client inizializzato\n\n");

    printf("******************* CLIENT AVVIATO *******************\n");
    memset(buffer, 0, DIM_BUFFER);
    /* Ricezione del messaggio iniziale con le istruzioni*/
    ret = recv(fdmax, buffer, DIM_BUFFER, 0);
    if(ret < 0){
        perror("ERR: Errore nella ricezione delle istruzioni");
        exit(1);
    }
    printf("%s\n"
            "******************************************************\n\n", buffer);
    

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(fdmax, &master);
    FD_SET(STDIN_FILENO, &master);

    for(;;) {
        memset(buffer, 0, DIM_BUFFER);
        read_fds = master;
        select(fdmax + 1, &read_fds, NULL, NULL, NULL);

        for(i = 0; i <= fdmax; i++) {
            /* Controllo dello STDIN per ricezione comandi da terminale del client */
            if(FD_ISSET(i, &read_fds)) {
                if(i == STDIN_FILENO) {
                    memset(buffer, 0, DIM_BUFFER);
                    fgets(buffer, DIM_BUFFER, stdin);
                    /* se siamo giocatori secondari e non usiamo il comando end*/
                    if(canSend == false  && strstr(buffer, "end") == NULL) {
                        printf("Ancora non sei stato interpellato dal giocatore principale\n");
                        continue;
                    }

                    /* eliminazione del carattere di newline e aggiunta dello spazio finale per strtok del messaggio lato server*/
                    strtok(buffer, "\n"); 
                    strcat(buffer, " ");

                    ret = send(fdmax, buffer, DIM_BUFFER, 0);
                    if(ret < 0){
                        perror("ERR: Errore durante l'invio del messaggio");
                        exit(1);
                    }

                    /* se al momento dell'inserimento del comando di start si sceglie il personaggio secondario blocchiamo l'invio al server, 
                    il client secondario deve interagire solo quando viene chiamata*/
                    if(strstr(buffer, "start") != NULL) {
                        strtok(buffer, " "); 
                        strcpy(choosenSet, strtok(NULL, " "));
                        strcpy(buffer, strtok(NULL, " "));
                        if(strcmp(buffer, "2") == 0)
                            canSend = false;
                        else 
                            canSend = true;    
                    }
                    
                }
                /* Altrimenti è il server che invia messaggi direttamente, come infos e join del giocatore secondario*/
                else if(i == fdmax) {
                    memset(buffer, 0, sizeof(buffer));
                    ret = recv(fdmax, buffer, DIM_BUFFER, 0);

                    if(ret == 0) {
                        printf("Disconnessione avvenuta\n");
                        exit(0);
                    }

                    if(ret < 0) {
                        perror("ERR: Errore in ricezione del messaggio");
                        exit(1);
                    }

                    /* reset del canSend in caso di chiamata in corso*/
                    if(strcmp(buffer, "chiamata in corso") == 0) {
                        canSend = true;
                        if(strcmp(choosenSet, "1") == 0) /* Prison Break*/
                            printf("\nUn prigioniero ti sta chiamando, quanti solti gli vuoi chiedere?\n");
                        /*else if(...) per gli altri scenari */
                    }  

                    /* nel caso di campo compilato non corretto si resetta sempre il canSend*/
                    if(strstr(buffer, "scenario non esistente") != NULL
                        || strstr(buffer, "giocatore inserito non esistente") != NULL
                        || strstr(buffer, "devi prima fare login") != NULL 
                        || strstr(buffer, "devi prima avviare una partita") != NULL
                        || strstr(buffer, "ALERT") != NULL) {

                        canSend = true;
                    }

                    printf("%s\n", buffer);
                }
            }
        } 
    } 
}