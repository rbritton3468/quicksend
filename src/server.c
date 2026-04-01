#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <string.h>


#define BUFFER_SZ 1024
#define LISTEN_BACKLOG 1

int server (int port) {
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddr_sizel = sizeof (clientAddr);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((short)port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	
	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	
	if(socket_fd == -1) {
		perror("Unaable to open socket\n");
		return -1;
	} if (bind (socket_fd, (struct sockaddr *)  &serverAddr, sizeof (serverAddr) ) == -1 ){
		perror("Unable to bind port to socket\n");
		return -1;
	} if (listen (socket_fd, LISTEN_BACKLOG) == -1){
		perror("Unable to listen on port\n");
		return -1;
	}
	int acc  = accept (socket_fd, (struct sockaddr *)  &clientAddr, &clientAddr_sizel);
	if(acc == -1){
		perror("error accepting connection on port");
		return -1;
	}

	char buffer[BUFFER_SZ];
	while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
		    send(acc, buffer, strlen(buffer), 0);
	}


return 0;
}


