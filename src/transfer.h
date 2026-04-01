#ifndef TRANSFER_H
#define TRANSFER_H

#include <sys/types.h>

#define BUFFER_SZ 1024

int send_from_stdin(int socket_fd);
int receive_to_stdout(int socket_fd);

#endif
