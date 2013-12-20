#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include "create_and_prepare_socket.h"
#define BACKLOG 4


int main() {

	register int s, c;
	int b;
	struct sockaddr_in sa;
	struct tm *tm;
	FILE *client;

	s = createSocket(s);
	sa = prepareSocketAddress(sa, 80);
	sa = doBind(s, sa);

	switch (fork()) {
		case -1:
			perror("fork");
			return 3;
			break;
		default:
			close(s);
			return 0;
			break;
		case 0:
			break;	
	}

	listen(s, BACKLOG);

	for(;;) {
		b = sizeof sa;

		if ((c = accept(s, (struct sockaddr *)&sa, &b)) < 0) {
			perror("mywebserver accept");
			return 4;
		}

		if ((client = fdopen(c, "w")) == NULL) {
			perror("mywebserver fdopen");
			return 5;
		}

		sendfile(
		 int
		  sendfile(int fd, int s, off_t offset, size_t nbytes,
				   struct sf_hdtr *hdtr, off_t *sbytes, int flags);


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
