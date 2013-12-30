#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "create_and_prepare_socket.h"
#define MAX_BUF 1024

static void
do_fork(int s) {
	switch (fork()) {
		case -1:
			perror("fork");
			exit(3);
		default:
			close(s);
			exit(0);
			break;
		case 0:
			break;
	}
}

int run_server() {
	register int s, c;
	int b;
	struct sockaddr_in sa;
	struct tm *tm;
	FILE *client;
	char buf[MAX_BUF];

	s = createSocket(s);
	sa = prepareSocketAddress(sa, 4321);
	sa = doBind(s, sa);
	do_fork(s);

	struct sockaddr_in client_name;
	int addrlen = sizeof(client_name);
	if (recvfrom(s, buf, MAX_BUF, 0,
		(struct sockaddr*)&client_name, &addrlen) < 0) {
		perror("recvfrom error");
		exit(6);
	}

	printf("%s", buf);
	strcat(buf, "OK!\n");

	if (sendto(s, buf, strlen(buf)+1, 0,
		(struct sockaddr*)&client_name, sizeof(client_name)) < 0) {
		perror("sendto error");
		exit(7);
	}
	close(s);

//	for(;;) {
//		b = sizeof sa;
//
//		if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) {
//			perror("accept error");
//			return 4;
//		}
//
//		if ((client = fdopen(c, "w")) == NULL) {
//			perror("fdopen error");
//			return 5;
//		}
//
//		fprintf(client,
//		"	<html>"
//		"		<head>"
//		"			<title>This is the title!</title>"
//		"		</head>"
//		"		<body>"
//		"			<h1>This is a header!</h1>"
//		"			<p>This is a paragraph!</p>"
//		"		</body>"
//		"	</html>"
//			);
//		fclose(client);
//	}

	return 0;
} //end of run_server


