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
#include "sendfile_wrapper.h"

#define SERVER_PORT_NUMBER 4321

/* size of messages for managing communication */
#define CTRL_MSG_SIZE 256 
#define OK_MSG 0
#define ACTION_CLIENT_WANTS_VIDEO 1
#define ACTION_CLIENT_SENDING_SERVER_VIDEO 2 
#define VIDEO_PATH_BASE "/usr/local/var/videos/%d"

#define ACTION_TELL_CLIENT_SERVER_IS_READY 3
#define BUF_SIZE 1024

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
    memset(buf, 0, max_buf_size);
    result = Read(socket, buf, max_buf_size);
    if (result > 0) { /* we read some bytes */
#ifdef DEBUG
      fprintf(stderr, "putting %d bytes into file\n", result);
#endif
      Write(file, buf, result);
    }
    if (result < 0) { /* -1 means general error */
      perror("read socket error");
      /* TODO - BK - 1/9/2014 - do we need a better way to exit? */
      exit(13); 
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
  char pathbuf[128];
  sprintf(pathbuf, VIDEO_PATH_BASE ,id);

#ifdef DEBUG
  fprintf(stderr, "\nSending requested file id[%d] to socket id [%d]\n",
	  id, socket);
  fprintf(stderr, "\nabout to open file at %s", pathbuf);
#endif

  myfile = Open(pathbuf, O_RDONLY | O_NONBLOCK);

#ifdef DEBUG
  fprintf(stderr, "\nSending file at %s to socket %d", pathbuf, socket);
#endif

  int sbytes = 0;
  int origin = 0;
  int nbytes = 1024;
  sbytes = sendfile_wrapper(myfile, socket, origin, nbytes);

#ifdef DEBUG
  fprintf(stderr, 
	  "\nSuccessfully sent file at "
	  "%s to socket %d, (%d bytes)", pathbuf, socket, sbytes);
#endif

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
  int file = Open(pathbuf, O_RDWR | O_CREAT);
  read_from_stream_into_file(socket, file);
  return 0;
}

int
handle_accepted_socket(int socket) {
  unsigned char buf[CTRL_MSG_SIZE];
  memset(buf, 0, CTRL_MSG_SIZE);
  Read(socket, buf, CTRL_MSG_SIZE);

  /*
    Text based packets, separated by colons.
    sessionid:action:entity id:userid:offset:sbytes:seq_num:timestamp:uip

    example:
    1028:get:94527:274:1828392:1024:3:1391276753:192.168.56.2

  */

  int action = buf[0];
  int id = 0;

  switch(action) {
  case ACTION_CLIENT_WANTS_VIDEO:
    id = buf[1];
    int nbytes = buf[2];
    int offset = buf[3];
    send_requested_file(socket, id);
    break;
  case ACTION_CLIENT_SENDING_SERVER_VIDEO:
    id = buf[1];
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


