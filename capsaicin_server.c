#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "create_and_prepare_socket.h"
#define MAX_BUF 1024

int run_server() {
	register int s;
	struct sockaddr_in sa;
	char buf[MAX_BUF];

	s = createSocket(s);
	sa = prepareSocketAddress(sa, 4321);
	sa = doBind(s, sa);

	struct sockaddr_in client_name;
	int addrlen = sizeof(client_name);
	for(;;){
		if (recvfrom(s, buf, MAX_BUF, 0,
			(struct sockaddr*)&client_name, &addrlen) < 0) {
			perror("recvfrom error");
			exit(6);
		}
		//sa.sin_addr.s_addr = htonl(INADDR_ANY);
		fprintf(stderr, "\nsending data to client at %d", client_name.sin_addr.s_addr);
		strcat(buf, "OK!\n");

		if (sendto(s, buf, strlen(buf)+1, 0,
			(struct sockaddr*)&client_name, sizeof(client_name)) < 0) {
			perror("sendto error");
			exit(7);
		}
	}

	return 0;
}


