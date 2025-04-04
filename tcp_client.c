#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_LINE 256

// main takes 3 arguments: the executable name, the server host (which will
// likely be 127.0.0.1 -- localhost) and the port of the server you are
// connecting to
int main(int argc, char* argv[]) {
    // a struct pointer that describes the internet host referenced by name. In
    // general has info about host
    struct hostent* hp;
    // host is a string representing the host ip address given by command line
    // arg
    // 2
    char* host;
    // port that for the server we will use which is given by command line arg 3
    int server_port;

    // a buffer or sending and recieving messages
    char buf[MAX_LINE];
    // the length of the message/buffer used
    int len;

    if (argc != 3) {
        printf("Usage: ./tcp_client <server_host>  <server_port>\n");
        return -1;
    } else {
        // save our command line args to variables
        host = argv[1];
        server_port = atoi(argv[2]);
    }

    // initialize our host info by looking up host ip address
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "tcp_client: unknown host %s\n", host);
        exit(1);
    }

    // our client will connect to a server with the following address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    // AF_INET is IPv4
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    strcpy(hp->h_addr_list[0], (char*)&server_address.sin_addr);

    // create socket
    int client_socket;

    // Initalize socket and do error checking
    // SOCK_STREAM is TCP
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
    }

    // attempt to connect to server
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

    close(client_socket);

    return 0;
}
