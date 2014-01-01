#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "ntwk_utils.h"
#define BACKLOG 4

int
Open(const char *path, int flags, ...) {
	int fd;
	if ((fd = open(path, flags)) < 0) {
		perror("open file error");
		exit(12);
	}
	return fd;
}

int
Socket(int domain, int type, int protocol) {
	int result;
	if ((result = socket(domain, type, protocol)) < 0) {
		perror("socket creation error");
		exit(1);
	}
	return result;
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

int
Bind(int s, const struct sockaddr *addr, socklen_t addrlen) {
	int result;
	if ((result = bind(s, addr, addrlen)) < 0) {
		perror("bind error");
		exit(2);
	}
	return result;
}

int
Listen(int s, int backlog) {
	int result;
	if ((result = listen(s, backlog)) < 0) {
		perror("listen error");
		exit(3);
	}
	return result;
}


int
Accept(int s, struct sockaddr * restrict addr, socklen_t * restrict addrlen) {
	int accepted_socket;
	if ((accepted_socket = accept(s, addr, addrlen)) < 0) {
		perror("accept error");
		exit(4);
	}
	return accepted_socket;
}


int
Sendfile(int fd, int s, off_t offset, size_t nbytes, struct sf_hdtr *hdtr, off_t *sbytes, int flags) {
	int result;
	if ((result = sendfile(fd, s, offset, nbytes, hdtr, sbytes, flags)) < 0) {
		perror("sendfile error");
		exit(5);
	}
	return result;
}

int run_server() {
	register int primary_socket, accepted_socket;
	socklen_t size_var;
	struct sockaddr_in sa;

	primary_socket = Socket(PF_INET, SOCK_STREAM, 0);
	sa = prepareSocketAddress(sa, 4321);
	Bind(primary_socket, (struct sockaddr *)&sa, sizeof sa);

    switch (fork()) {
        case -1:
            perror("fork");
            return 3;
            break;
        default:
            close(primary_socket);
            return 0;
            break;
        case 0:
            break;
    }

    Listen(primary_socket, BACKLOG);

    for (;;) {
        size_var = sizeof sa;
        accepted_socket = Accept(primary_socket, (struct sockaddr *)&sa, &size_var);
		int myfile = Open("/usr/home/byron/binary_file", O_RDONLY);

		 off_t sbytes;
		 Sendfile(myfile,
					accepted_socket, /* socket */
					0, /* offset of 0 */
					0, /* number of bytes, 0 being send whole file */
					NULL, /* no header needed yet */
					&sbytes, /* we'll get the number of bytes sent */
					0 /* no flags */
					);
    }

	return 0;
}


