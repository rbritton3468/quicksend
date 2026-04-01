#include "stdio.h"
#include "server.c"
#include "client.c"

int main (int argc, char** argv) {


if (argc == 2) {//case: server
	return server(9999);

} else if (argc == 3) {//case: client 
	char* addr = "127.0.0.1";
	return client(addr,9999);

} else {
	printf("usage: \n1. server: quicksend <port> \n2. client: quicksend <server IP> <server port>\n");
}


}
