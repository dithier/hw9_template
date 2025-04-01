#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// some types and uniz operations
#include <sys/types.h>
#include <unistd.h>

// a sockets library and internet protocol library
#include <netinet/in.h>
#include <sys/socket.h>

// max number of client connections that can be queued to join
#define MAX_CLIENTS 5
#define MAX_LINE 256

int main(int argc, char *argv[]) {
    int port;
    int client_socket;
    int addr_len;
    char buf[MAX_LINE];
    int buf_len;

    if (argc != 2) {
        printf("Usage ./tcp_server <port number>\n");
        return -1;
    } else {
        port = atoi(argv[1]);
    }

    // define the server address format
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // create the server socket
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket failed\n");
    }

    // bin dthe server
    if ((bind(server_socket, (struct sockaddr *)&server_address,
              sizeof(server_address))) < 0) {
        printf("failure on bind\n");
        exit(1);
    } else {
        printf("Server bind()\n");
    }

    // set up listening
    listen(server_socket, MAX_CLIENTS);

    while (1) {
        if ((client_socket =
                 accept(server_socket, (struct sockaddr *)&server_address,
                        (socklen_t *)&addr_len)) < 0) {
            printf("connection error");
            return 0;
        }

        while (((buf_len) = recv(client_socket, buf, sizeof(buf), 0))) {
            fputs(buf, stdout);
            send(client_socket, buf, strlen(buf) + 1, 0);
        }
    }

    close(server_socket);
    return 0;
}
