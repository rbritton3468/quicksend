#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "transfer.h"

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
	close(socket_fd);

	pid_t parent_pid = getpid();
	pid_t pid = fork();
	if (pid == -1) {
		perror("unable to fork");
		close(acc);
		return -1;
	}

	if (pid == 0) {
		if (receive_to_stdout(acc) == -1) {
			close(acc);
			return -1;
		}

		kill(parent_pid, SIGTERM);
		close(acc);
		return 0;
	}

	if (send_from_stdin(acc) == -1) {
		close(acc);
		kill(pid, SIGTERM);
		waitpid(pid, NULL, 0);
		return -1;
	}

	shutdown(acc, SHUT_RDWR);
	kill(pid, SIGTERM);
	waitpid(pid, NULL, 0);
	close(acc);

return 0;
}
