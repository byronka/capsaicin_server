#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "basic_io.h"

int
Socket(int domain, int type, int protocol) {
	int result;
	if ((result = socket(domain, type, protocol)) < 0) {
		perror("socket creation error");
		exit(5);
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
		exit(4);
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
		exit(2);
	}
	return accepted_socket;
}

int
simple_sendfile(int fd, int s, int offset, int nbytes, off_t *sbytes) {
	int result;
	result = Sendfile(
		fd,
		s,
		0, /* offset of 0 */
		nbytes, /* number of bytes, 0 being send whole file */
		NULL, /* no header needed yet */
		sbytes, /* we'll get the number of bytes sent */
		0 /* no flags */
		);
	Close(fd);
	return result;
}

int
Sendfile(int fd, int s, off_t offset, size_t nbytes, 
		struct sf_hdtr *hdtr, off_t *sbytes, int flags) {
	int result;
	if ((result = sendfile(fd, s, offset, nbytes, hdtr, sbytes, flags)) < 0) {
		perror("\nsendfile error");
		fprintf(stderr, "sendfile error; inputs fd[%d] s[%d] offset[%lld]"
				"nbytes[%d] hdtr[%p] sbytes[%lld] flags[%d]",
				fd, s, offset, nbytes, (void *)hdtr, *sbytes, flags);
	}
	return result;
}

int
Close(int fd) {
	int result;
	if ((result = close(fd)) < 0) {
		perror("close file error");
		exit(6);
	}
	return result;
}

int
Open(const char *path, int flags, ...) {
	int fd;
	if ((fd = open(path, flags, 0700)) < 0) {
		perror("open file error");
		fprintf(stderr, "path was %s\n", path);
		fprintf(stderr, "flags were %d\n", flags);
		exit(7);
	}
	return fd;
}

ssize_t
Write(int fd, const void *buf, size_t nbytes) {
	ssize_t result;
	if ((result = write(fd, buf, nbytes)) < 0) {
		perror("write error");
		fprintf(stderr, "\nfd:[%d] &buf[%p] nbytes[%d]", fd, buf, nbytes );
	}
	return result;
}

ssize_t
Read(int fd, void *buf, size_t nbytes) {
	ssize_t result;
	if ((result = read(fd, buf, nbytes)) < 0) {
		perror("read error");
		fprintf(stderr, "\nfd:[%d] &buf[%p] nbytes[%d]", fd, buf, nbytes );
	}
	return result;
}
