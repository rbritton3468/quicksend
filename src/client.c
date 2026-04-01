#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "transfer.h"

int client (char* addr, int port) {
	struct sockaddr_in serverAddr;

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

	pid_t parent_pid = getpid();
	pid_t pid = fork();
	if (pid == -1) {
		perror("unable to fork");
		close(socket_fd);
		return -1;
	}

	if (pid == 0) {
		if (receive_to_stdout(socket_fd) == -1) {
			close(socket_fd);
			return -1;
		}

		kill(parent_pid, SIGTERM);
		close(socket_fd);
		return 0;
	}

	if (send_from_stdin(socket_fd) == -1) {
		close(socket_fd);
		kill(pid, SIGTERM);
		waitpid(pid, NULL, 0);
		return -1;
	}

	shutdown(socket_fd, SHUT_RDWR);
	kill(pid, SIGTERM);
	waitpid(pid, NULL, 0);
	close(socket_fd);
return 0;
}
