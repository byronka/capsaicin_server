#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "create_and_prepare_socket.h"
#include "ntwk_utils.h"

#define MAX_BUF 1024

void
print_buf_sample(unsigned char* buf) {
	int index;
	for (index = 0; index < 128; index++) {
		fprintf(stderr, "%d ", buf[index]);
	}
	fprintf(stderr, "...\n");
}

ssize_t
Sendto(int s, const void *msg, size_t len, int flags,
	const struct sockaddr *to, socklen_t tolen) {
	ssize_t result;
	if ((result = sendto(s, msg, len, flags, to, tolen)) < 0) {
		perror("sendto error");
		exit(7);
	}
	return result;
}

int
Open(const char *path, int flags, ...) {
	int fd;
	if ((fd = open(path, flags)) < 0) {
		perror("open file error");
		exit(12);
	}
	return fd;
}

ssize_t
Recvfrom(int s, void * restrict buf, size_t len, int flags, struct sockaddr * restrict from, socklen_t * restrict fromlen) {
	ssize_t result;
	if ((result = recvfrom(s, buf, len, flags, from, fromlen)) < 0) {
		perror("recvfrom error");
		exit(6);
	}
	return result;
}

int run_server() {
	register int s_udp;
	register int s_tcp;
	struct sockaddr_in sa;
	unsigned char buf[MAX_BUF];

	s_udp = createSocket_udp(s_udp);
	sa = prepareSocketAddress(sa, 4321);
	sa = doBind(s_udp, sa);

	struct sockaddr_in client_name;
	socklen_t addrlen = sizeof(client_name);
	for(;;){

		/* receive from client */

		bzero(buf, MAX_BUF); /* is it necessary to zero this out? */
		Recvfrom(s_udp, buf, MAX_BUF, 0, (struct sockaddr*)&client_name, &addrlen);

		/* print info on client */

		uint32_t client_address = ntohl(client_name.sin_addr.s_addr);
		unsigned char* client_addr_c = convert_ip_addr(client_address);
		fprintf(stderr, "\nsending data to client at ");
		print_ip_addr_32_bit(client_addr_c);

		/* open the file we are sending */
		int myfile;
		myfile = Open("/usr/home/byron/binary_file", O_RDONLY);

		/* start loop to send full contents via UDP */
		while(1) { /* will break out if eof, exit() otherwise */
			bzero(buf, MAX_BUF);
			/* move info from file into buffer */

			int result = read(myfile, buf, MAX_BUF);
#ifdef DEBUG
fprintf(stderr, "reading from fd %d, result: %d\n", myfile, result);
print_buf_sample(buf);
#endif
			if (result > 0) { /* some bytes to send */
				/* duplicate send to client START*/

				Sendto(s_udp, buf, MAX_BUF, 0,
					(struct sockaddr*)&client_name, sizeof(client_name));
#ifdef DEBUG
fprintf(stderr, "sending...\n");
print_buf_sample(buf);
#endif
				/* send to client END*/
			}
			if (result == 0) { /* returning 0 means eof */
#ifdef DEBUG
fprintf(stderr, "closing fd\n");
#endif
				if (close(myfile) < 0) {
					perror("close file error");
					exit(14);
				}
				break; /* here, we leave the loop because of eof*/
			}
			if (result < 0) { /* -1 means general error */
				perror("read file error");
				exit(13);
			}

			/* send to client START*/

			Sendto(s_udp, buf, MAX_BUF, 0, 
				(struct sockaddr*)&client_name, sizeof(client_name));
#ifdef DEBUG
fprintf(stderr, "sending...\n");
print_buf_sample(buf);
#endif
			/* send to client END*/
		}
	}

	return 0;
}


