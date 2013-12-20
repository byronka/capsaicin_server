#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>

int 
createSocket(int s) {
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	return s;
}

struct sockaddr_in
prepareSocketAddress(struct sockaddr_in sa, int port) {
	bzero(&sa, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (INADDR_ANY) {
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	return sa;
}

struct sockaddr_in
doBind(int s, struct sockaddr_in sa) {
	if (bind(s, (struct sockaddr *)&sa, sizeof sa) < 0) {
		perror("bind");
		exit(2);
	}
	return sa;
}
