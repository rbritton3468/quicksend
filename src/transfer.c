#include "transfer.h"

#include <sys/socket.h>
#include <unistd.h>

static int write_all(int fd, const char *buffer, ssize_t buffer_size) {
	ssize_t total_written = 0;

	while (total_written < buffer_size) {
		ssize_t bytes_written = write(fd, buffer + total_written, buffer_size - total_written);
		if (bytes_written <= 0) {
			return -1;
		}
		total_written += bytes_written;
	}

	return 0;
}

static int send_all(int socket_fd, const char *buffer, ssize_t buffer_size) {
	ssize_t total_sent = 0;

	while (total_sent < buffer_size) {
		ssize_t bytes_sent = send(socket_fd, buffer + total_sent, buffer_size - total_sent, 0);
		if (bytes_sent <= 0) {
			return -1;
		}
		total_sent += bytes_sent;
	}

	return 0;
}

int send_from_stdin(int socket_fd) {
	char buffer[BUFFER_SZ];
	ssize_t bytes_read;

	while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
		if (send_all(socket_fd, buffer, bytes_read) == -1) {
			return -1;
		}
	}

	if (bytes_read < 0) {
		return -1;
	}

	return 0;
}

int receive_to_stdout(int socket_fd) {
	char buffer[BUFFER_SZ];
	ssize_t bytes_received;

	while ((bytes_received = recv(socket_fd, buffer, BUFFER_SZ, 0)) > 0) {
		if (write_all(STDOUT_FILENO, buffer, bytes_received) == -1) {
			return -1;
		}
	}

	if (bytes_received < 0) {
		return -1;
	}

	return 0;
}
