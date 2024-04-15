// Server side C program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int server_fd, new_socket;
	ssize_t valread;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR, &opt,
				sizeof(opt))) {
		fprintf(stderr, "setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		fprintf(stderr, "bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		fprintf(stderr, "listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				&addrlen))
		< 0) {
		fprintf(stderr, "accept");
		exit(EXIT_FAILURE);
	}
	
    const int BUFFER_SIZE = 1000000;
    unsigned char buffer[BUFFER_SIZE];

    FILE* file = fopen( "result", "wb" );
    int size;
    while ((size = recv(new_socket, buffer, BUFFER_SIZE - 1, 0)) >= 0)
        if (size > 0 && size < BUFFER_SIZE)
            fwrite( buffer, 1, size, stdout );

	return 0;
}
