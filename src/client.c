#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <unistd.h>
#include <arpa/inet.h>


#define BUFFER_SZ 1024

int client (char* addr, int port) {
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddr_sizel = sizeof (clientAddr);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((short)port);
	serverAddr.sin_addr.s_addr = inet_addr(addr);

	
	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	if(socket_fd == -1) {
		perror("Unaable to open socket\n");
		return -1;
	} 

	int acc  = connect (socket_fd, (struct sockaddr *)  &serverAddr, sizeof(serverAddr));
	if(acc == -1){
		perror("error accepting connection on port");
		return -1;
	}


	char buffer[BUFFER_SZ];
	ssize_t bytes_received;

	while ((bytes_received = recv(socket_fd, buffer, BUFFER_SZ, 0)) > 0) {
    		// Write received data directly to stdout (file descriptor 1)
    		write(STDOUT_FILENO, buffer, bytes_received);
	}	
	close(socket_fd);
return 0;
}


