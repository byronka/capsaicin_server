#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "create_and_prepare_socket.h"
#define BACKLOG 4


void do_fork(int s);
int run_server();


int run_server() {

	register int s, c;
	int b;
	struct sockaddr_in sa;
	struct tm *tm;
	FILE *client;

	s = createSocket(s);
	sa = prepareSocketAddress(sa, 4321);
	sa = doBind(s, sa);
	do_fork(s);
	listen(s, BACKLOG);

	for(;;) {
		b = sizeof sa;

		if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) {
			perror("accept");
			return 4;
		}

		if ((client = fdopen(c, "w")) == NULL) {
			perror("fdopen");
			return 5;
		}

		fprintf(client,
		"	<html>"
		"		<head>"
		"			<title>This is the title!</title>"
		"		</head>"
		"		<body>"
		"			<h1>This is a header!</h1>"
		"			<p>This is a paragraph!</p>"
		"		</body>"
		"	</html>"
			);
		fclose(client);
	}
}

void
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
