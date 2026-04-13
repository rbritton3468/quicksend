#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "transfer.h"

#define LISTEN_BACKLOG 1

int server (int port) {
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddr_sizel = sizeof(clientAddr);

	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((short)port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1) {
		perror("Unaable to open socket\n");
		return -1;
	}
	if (bind(socket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
		perror("Unable to bind port to socket\n");
		close(socket_fd);
		return -1;
	}
	if (listen(socket_fd, LISTEN_BACKLOG) == -1) {
		perror("Unable to listen on port\n");
		close(socket_fd);
		return -1;
	}

	int acc = accept(socket_fd, (struct sockaddr *) &clientAddr, &clientAddr_sizel);
	if(acc == -1) {
		perror("error accepting connection on port");
		close(socket_fd);
		return -1;
	}
	close(socket_fd);

	pid_t pid = fork();
	if (pid == -1) {
		perror("unable to fork");
		close(acc);
		return -1;
	}

	if (pid == 0) {
		int receive_rc = receive_to_stdout(acc);
		close(acc);
		return receive_rc;
	}

	int send_rc = send_from_stdin(acc);
	if (shutdown(acc, SHUT_WR) == -1) {
		perror("shutdown write side");
	}

	int status = 0;
	int wait_rc;
	do {
		wait_rc = waitpid(pid, &status, 0);
	} while (wait_rc == -1 && errno == EINTR);

	close(acc);

	if (send_rc == -1) {
		return -1;
	}
	if (wait_rc == -1) {
		return -1;
	}
	if (WIFEXITED(status)) {
		return WEXITSTATUS(status);
	}

	return -1;
}
