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

/* size of messages for managing communication */
#define CTRL_MSG_SIZE 256 
#define VIDEO_PATH_BASE "/usr/local/var/videos/%d"

static int read_from_stream_into_file(int socket, int file);
static int send_requested_file(int socket, int id);
static int receive_file(int socket, int id);
static int handle_accepted_socket(int socket);

/* Note: this program, request_handler, does not know about anything other
than the circular ctlmsg buffer queue.  It subscribes for any new items
on the buffer.  This keeps it up to date whether there is anything 
available waiting on that queue.  If it is free and knows there is 
something waiting on the buffer, it will try to take it.

The part of this program that accepts messages about the circular buffer
needs to be a separate thread.
*/
extern int
init_request_handler() {
 //open socket to circ_ctlmsg_buffer
 //subscribe to it
}

static int
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

static int
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

static int
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

static int
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
