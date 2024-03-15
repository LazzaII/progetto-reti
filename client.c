
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

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

    /* P principale - S secondario - A attivi a rispondere - N non settato*/
    char type[2];
    strcpy(type, "N"); 

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
                    /* se siamo giocatori secondari */
                    if(strcmp(type, "S") == 0) {
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
                        strtok(NULL, " "); 
                        strtok(NULL, " ");
                        if(strcmp(buffer, "2") == 0) 
                            strcpy(type, "S"); 
                        else 
                            strcpy(type, "P"); 
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

                    /* TODO: sbloccare client quando c'è la chiamata
                    if() {

                    }  */

                    printf("%s\n", buffer);
                }
            }
        } 
    } 
}