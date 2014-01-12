#include <stdio.h>
#include <sys/stat.h>
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

#define SERVER_PORT_NUMBER 4321

 /* size of messages for managing communication */
#define CTRL_MSG_SIZE 16 
#define ACTION_CLIENT_WANTS_VIDEO 1
#define ACTION_CLIENT_SENDING_SERVER_VIDEO 2 
#define VIDEO_PATH_BASE "/usr/local/var/videos/%d"

#define ACTION_TELL_CLIENT_SERVER_IS_READY 3

static int read_from_stream_into_file(int socket, int file);
static int send_requested_file(int socket, int id);
static int receive_file(int socket, int id);
static int handle_accepted_socket(int socket);
static int fork_server(int s, struct sockaddr_in sa);
extern int run_server();

int
read_from_stream_into_file(int socket, int file) {
#ifdef DEBUG
	fprintf(stdout, "receiving file id[%d] from socket id [%d]\n",
			file, socket);
#endif
	ssize_t max_buf_size = 1024;
	unsigned char buf[max_buf_size];
	int result;
	do {
		bzero(buf, max_buf_size);
		result = Read(socket, buf, max_buf_size);
		if (result > 0) { /* we read some bytes */
#ifdef DEBUG
			fprintf(stderr, "putting %d bytes into file\n", result);
#endif
			Write(file, buf, result);
		}
		if (result < 0) { /* -1 means general error */
				perror("read socket error");
				exit(13); /* TODO - BK - 1/9/2014 - do we need a better way to exit? */
		}
	} while(result != 0);  /* returning 0 means no more bytes to write */
#ifdef DEBUG
	fprintf(stderr, "closing the file\n");
#endif
	Close(file);
	return 0; /* TODO - BK no return info implemented */
}

int
send_requested_file(int socket, int id) {
	int myfile;
#ifdef DEBUG
	fprintf(stdout, "Sending requested file id[%d] to socket id [%d]\n",
			socket, id);
#endif

	/* TODO - BK - 1/11/14 - need to make sure below line is safe */
			
	char pathbuf[128];
	sprintf(pathbuf, VIDEO_PATH_BASE ,id);
	myfile = Open(pathbuf, O_RDONLY);

	off_t sbytes;
	simple_sendfile(myfile, socket, &sbytes);
	return 0;
}

int
receive_file(int socket, int id) {
	unsigned char msg = ACTION_TELL_CLIENT_SERVER_IS_READY;
	Write(socket, &msg, sizeof(&msg));
#ifdef DEBUG
	fprintf(stderr, "telling client that server is ready to receive file\n");
#endif

	/* receive file */

	char pathbuf[128];
	sprintf(pathbuf, VIDEO_PATH_BASE,id);
	int file = Open(pathbuf, O_RDWR | O_CREAT | O_APPEND);
	read_from_stream_into_file(socket, file);
	return 0;
}

int
handle_accepted_socket(int socket) {
	unsigned char buf[CTRL_MSG_SIZE];
	Read(socket, buf, CTRL_MSG_SIZE);

	/*we parse the bytes: first byte is the action, following are 
	  the id, if any */

    int action = buf[0];
	int id = buf[1];

	switch(action) {
		case ACTION_CLIENT_WANTS_VIDEO:
			send_requested_file(socket, id);
			break;
		case ACTION_CLIENT_SENDING_SERVER_VIDEO:
			receive_file(socket, id);
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
fork_server(int s, struct sockaddr_in sa) {
	socklen_t size_var;
	int accepted_socket;
    switch (fork()) {
        case -1:
            perror("fork");
            exit(3);
            break;
        default:
            Close(s);
            return 0;
            break;
        case 0:
            break;
    }

    Listen(s, BACKLOG);

    for (;;) {
        size_var = sizeof sa;
        accepted_socket = Accept(s, (struct sockaddr *)&sa, &size_var);
		handle_accepted_socket(accepted_socket);
    }
}


int run_server() {
	int primary_socket;
	struct sockaddr_in sa;
	primary_socket = Socket(PF_INET, SOCK_STREAM, 0);
	sa = prepareSocketAddress(sa, SERVER_PORT_NUMBER);
	Bind(primary_socket, (struct sockaddr *)&sa, sizeof sa);
	fork_server(primary_socket, sa);
	return 0;   /* no return code implemented */
}


