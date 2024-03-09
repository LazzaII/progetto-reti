
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
#define COMANDO_DIM 64

int main(int argc, char *argv[])
{
    int ret, i, cl_sock;
    fd_set  master, read_fds;
    char buffer[DIM_BUFFER];
    struct sockaddr_in sv_addr;
    in_port_t porta = htons(SERVER_PORT);

    printf("! Avvio e inizializzazione del client in corso...\n\n");
    cl_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(cl_sock < 0) {
        perror("ERR: Errore creazione socket");
        exit(1);
    }

    memset(&sv_addr, 0, sizeof(sv_addr));
    sv_addr.sin_family = AF_INET;
    sv_addr.sin_port = porta;
    inet_pton(AF_INET, SERVER_IP, &sv_addr.sin_addr);

    ret = connect(cl_sock, (struct sockaddr*)&sv_addr, sizeof(sv_addr));
    if(ret < 0) {
        perror("ERR: Errore nella connessione con il server");
        exit(1);
    }
    printf("OK: Client inizializzato\n\n");

    printf("******************* CLIENT AVVIATO *******************\n");
    memset(buffer, 0, sizeof(buffer));
    /* Ricezione del messaggio iniziale con le istruzioni*/
    ret = recv(cl_sock, buffer, DIM_BUFFER, 0);
    if(ret < 0){
        perror("ERR: Errore nella ricezione delle istruzioni");
        exit(1);
    }
    printf("%s"
            "\n******************************************************\n\n", buffer);
    

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(cl_sock, &master);
    FD_SET(STDIN_FILENO, &master);

    for(;;) {
        memset(buffer, 0, sizeof(buffer));
        read_fds = master;
        select(cl_sock + 1, &read_fds, NULL, NULL, NULL);

        for(i = 0; i <= cl_sock; i++) {
            /* Controllo dello STDIN per ricezione comandi da terminale del client */
            if(FD_ISSET(i, &read_fds)) {
                if(i == STDIN_FILENO) {
                    fgets(buffer, COMANDO_DIM, stdin);
                    /* elimino carattere \n messo dalla fgets */
                    buffer[strcspn(buffer, "\n")] = 0; 
                    ret = send(cl_sock, buffer, DIM_BUFFER, 0);
                    if(ret < 0){
                        perror("ERR: Errore durante l'invio del messaggio");
                        exit(1);
                    }

                    /* controllare se si è giocatore secondario per bloccare lo STDIN finchè non arriva la chiamata */

                    memset(buffer, 0, sizeof(buffer));
                    ret = recv(cl_sock, buffer, DIM_BUFFER, 0);
                    if(ret < 0){
                        perror("ERR: Errore in ricezione del messaggio");
                    }
                    printf("%s", buffer);
                }
                /* Altrimenti è il server che invia messaggi direttamente, come infos e join del giocatore secondario*/
                else if(i == cl_sock) {
                    memset(buffer, 0, sizeof(buffer));
                    ret = recv(cl_sock, buffer, DIM_BUFFER, 0);
                    if(ret < 0){
                        perror("ERR: Errore in ricezione del messaggio");
                    }
                    printf("%s", buffer);
                }
            }
        } 
    } 
}