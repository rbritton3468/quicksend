#include "sys/socket.h"
#include "netinet/in.h"
#include "stdio.h"	
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
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

	if (connect(socket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
		perror("error accepting connection on port");
		close(socket_fd);
		return -1;
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("unable to fork");
		close(socket_fd);
		return -1;
	}

	if (pid == 0) {
		int receive_rc = receive_to_stdout(socket_fd);
		close(socket_fd);
		return receive_rc;
	}

	int send_rc = send_from_stdin(socket_fd);
	if (shutdown(socket_fd, SHUT_WR) == -1) {
		perror("shutdown write side");
	}

	int status = 0;
	int wait_rc;
	do {
		wait_rc = waitpid(pid, &status, 0);
	} while (wait_rc == -1 && errno == EINTR);

	close(socket_fd);

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
