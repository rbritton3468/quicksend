#include "stdio.h"
#include "stdlib.h"
#include "transfer.c"
#include "server.c"
#include "client.c"

int main (int argc, char** argv) {


if (argc == 2) {//case: server
	return server(atoi(argv[1]));

} else if (argc == 3) {//case: client 
	return client(argv[1], atoi(argv[2]));

} else {
	printf("usage: \n1. server: quicksend <port> \n2. client: quicksend <server IP> <server port>\n");
}


}
