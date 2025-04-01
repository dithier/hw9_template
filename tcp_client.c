#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LINE 256

int main(int argc, char* argv[]) {
    struct hostent* hp;
    char* host;
    // port that we will use
    int server_port;

    // define other variables used for later
    char buf[MAX_LINE];
    int len;

    if (argc != 3) {
        printf("Usage: ./tcp_client <server_host>  <server_port>\n");
        return -1;
    } else {
        host = argv[1];
        server_port = atoi(argv[2]);
    }

    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "tcp_client: unknwon host %s\n", host);
        exit(1);
    }

    // our client will connect to a server with the following address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    strcpy(hp->h_addr_list[0], (char*)&server_address.sin_addr);

    // create socket
    int client_socket;

    // Initalize socket and do error checking
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
    }

    // attempt to connect
    int connection_status;

    if ((connection_status =
             connect(client_socket, (struct sockaddr*)&server_address,
                     sizeof(server_address))) < 0) {
        printf("%d Connection unsuccessful\n", connection_status);
        close(client_socket);
        exit(0);
    } else {
        printf("Connected succesfully\n");
    }

    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE - 1] = '\0';
        len = strlen(buf) + 1;

        // send a message to the server
        send(client_socket, buf, len, 0);

        // receive a message from the server
        len = recv(client_socket, buf, sizeof(buf), 0);
        fputs("Server says: ", stdout);
        fputs(buf, stdout);
    }

    return 0;
}
