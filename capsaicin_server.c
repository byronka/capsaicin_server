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

int run_server() {
	register int s_udp;
	register int s_tcp;
	struct sockaddr_in sa;
	char buf[MAX_BUF];

	s_udp = createSocket_udp(s_udp);
	sa = prepareSocketAddress(sa, 4321);
	sa = doBind(s_udp, sa);

	struct sockaddr_in client_name;
	socklen_t addrlen = sizeof(client_name);
	for(;;){

		/* receive from client */

		bzero(buf, MAX_BUF); /* is it necessary to zero this out? */
		if (recvfrom(s_udp, buf, MAX_BUF, 0,
			(struct sockaddr*)&client_name, &addrlen) < 0) {
			perror("recvfrom error");
			exit(6);
		}

		/* print info on client */

		uint32_t client_address = ntohl(client_name.sin_addr.s_addr);
		unsigned char* client_addr_c = convert_ip_addr(client_address);
		fprintf(stderr, "\nsending data to client at ");
		print_ip_addr_32_bit(client_addr_c);

		/* open the file we are sending */
		int myfile;
		if ((myfile = open("/usr/home/byron/binary_file", O_RDONLY)) < 0) {
			perror("open file error");
			exit(12);
		}

		/* start loop to send full contents via UDP */
		while(1) { /* will break out if eof, exit() otherwise */
			bzero(buf, MAX_BUF);
			/* move info from file into buffer */
			int result = read(myfile, &buf, MAX_BUF);
#ifdef DEBUG
fprintf(stderr, "reading from fd %d, result: %d\n", myfile, result);
#endif
			if (result == 0) { /* returning 0 means eof */
#ifdef DEBUG
fprintf(stderr, "closing fd\n");
#endif
				if (close(myfile) < 0) {
					perror("close file error");
					exit(14);
				}
				/* duplicate send to client START*/

				if (sendto(s_udp, buf, strlen(buf)+1, 0,
					(struct sockaddr*)&client_name, sizeof(client_name)) < 0) {
					perror("sendto error");
					exit(7);
				}
				/* send to client END*/
				break; /* here, we leave the loop because of eof*/
			}
			if (result < 0) { /* -1 means general error */
				perror("read file error");
				exit(13);
			}

			/* send to client START*/

			if (sendto(s_udp, buf, strlen(buf)+1, 0,
				(struct sockaddr*)&client_name, sizeof(client_name)) < 0) {
				perror("sendto error");
				exit(7);
			}
			/* send to client END*/
		}
	}

	return 0;
}


