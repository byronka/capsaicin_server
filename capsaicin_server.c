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
#include "basic_io.h"


#define CTRL_MSG_SIZE 16 /* size of messages for managing communication */


#define CLIENT_WANTS_VIDEO 1 /* the client wants a video */
#define CLIENT_SENDING_SERVER_VIDEO 2 /* the client wants to send the server a video*/
#define SEND_SERVER_VIDEO 3
#define SEND_CLIENT_VIDEO 4

int
read_from_stream_into_file(int socket, int file) {
	ssize_t max_buf_size = 1024;
	unsigned char buf[max_buf_size];
	int result;
	do {
		bzero(buf, max_buf_size);
		result = Read(socket, buf, max_buf_size);
		if (result > 0) { /* we read some bytes */
			fprintf(stderr, "putting %d bytes into file\n", result);
			Write(file, buf, result);
		}
		if (result < 0) { /* -1 means general error */
				perror("read socket error");
				exit(13); /* TODO - BK - 1/9/2014 - do we need a better way to exit? */
		}
	} while(result != 0);  /* returning 0 means no more bytes to write */
	fprintf(stderr, "closing the file\n");
	Close(file);
	return 0; /* TODO - BK no return info implemented */
}

int
send_requested_file(int socket, int id) {
	/* TODO - BK - hardcoded for now, not fully implemented */
	int myfile;
	if (id != 5) {
		fprintf(stderr, "did not receive valid id\n");
		return -1;
	}

	myfile = Open("/usr/home/byron/binary_file", O_RDONLY);

	off_t sbytes;
	simple_sendfile(myfile, socket, &sbytes);
	return 0;
}

int
receive_file(int socket) {
	/* reply to client we are ready */
	unsigned char msg = SEND_SERVER_VIDEO;
	Write(socket, &msg, sizeof(&msg));

	/* receive file */
	int file = Open("/usr/home/byron/test_receive", O_RDWR | O_CREAT); /* TODO - BK - hardcoded for now - fix! */
	read_from_stream_into_file(socket, file);
	return 0;
}

int
handle_accepted_socket(int socket) {
	unsigned char buf[CTRL_MSG_SIZE];
	Read(socket, buf, CTRL_MSG_SIZE);

	/*we parse the bytes: first byte is the action, following are the id, if any */
    int action = buf[0];
	int id = buf[1];

	switch(action) {
		case CLIENT_WANTS_VIDEO:
			send_requested_file(socket, id);
			break;
		case CLIENT_SENDING_SERVER_VIDEO:
			receive_file(socket);
			break;
		default:
			fprintf(stderr, "error: no action sent. buffer[0] is %u and "
					"buffer[1] is %u. aborting\n", buf[0], buf[1]);
			exit(12);
	}
	
	Close(socket);
	return 0;
}

int
fork_server(int primary_socket, struct sockaddr_in sa) {
	socklen_t size_var;
	int accepted_socket;
    switch (fork()) {
        case -1:
            perror("fork");
            exit(3);
            break;
        default:
            Close(primary_socket);
            return 0;
            break;
        case 0:
            break;
    }

    Listen(primary_socket, BACKLOG);

    for (;;) {
        size_var = sizeof sa;
        accepted_socket = Accept(primary_socket, (struct sockaddr *)&sa, &size_var);
		handle_accepted_socket(accepted_socket);
    }
}


int run_server() {
	int primary_socket;
	struct sockaddr_in sa;
	primary_socket = Socket(PF_INET, SOCK_STREAM, 0);
	sa = prepareSocketAddress(sa, 4321);
	Bind(primary_socket, (struct sockaddr *)&sa, sizeof sa);
	fork_server(primary_socket, sa);
	return 0;   /* no return code implemented */
}


