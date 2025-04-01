CXX = clang -g -Wall -Werror

all: tcp_server.o tcp_client.o

tcp_server.o: tcp_server.c
	$(CXX) tcp_server.c -o tcp_server.o

tcp_client.o: tcp_client.c
	$(CXX) tcp_client.c -o tcp_client.o

clean:
	rm -f *.o
