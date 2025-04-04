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
// max buffer size for message
#define MAX_LINE 256

// main takes 2 args: the executable name and the port number
// for the server
int main(int argc, char *argv[]) {
    // port we are using for communication
    int port;
    // the socket we will use to communicate with client
    int client_socket;
    // the amount of space pointed to by the address in the
    // accept call and on return will have th actual length (in bytes)
    // of the address returned (see use in while loop below)
    int addr_len;
    // the buffer we will be storing the client's request in
    char buf[MAX_LINE];
    // the length of the buffer used that  we will be storing the client's
    // request in
    int buf_len;

    if (argc != 2) {
        printf("Usage ./tcp_server <port number>\n");
        return -1;
    } else {
        port = atoi(argv[1]);
    }

    // define the server address format
    // server_address is struct to store server details
    struct sockaddr_in server_address;
    // AF_INET specifies IPv4
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    // INADDR_ANY allows server to accept connections on any available network
    // interface
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // create the server socket
    int server_socket;
    // SOCK_STREAM is TCP
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket failed\n");
    }

    // bind the socket with specified ip address and port
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
        // accept waits for client connections
        // if client connects we return a new socket descripter (client socket)
        if ((client_socket =
                 accept(server_socket, (struct sockaddr *)&server_address,
                        (socklen_t *)&addr_len)) < 0) {
            printf("connection error");
            return 0;
        }

        // receive data and store it in buf
        while (((buf_len) = recv(client_socket, buf, sizeof(buf), 0))) {
            // print message to console
            fputs(buf, stdout);
            // send received message back to client
            send(client_socket, buf, strlen(buf) + 1, 0);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
