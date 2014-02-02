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
#define ACTION_TELL_CLIENT_SERVER_IS_READY 3
#define BUF_SIZE 1024

static int fork_server(int s, struct sockaddr_in sa);
extern int run_server();

static int
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

/* initialization code */
extern int 
run_server() {
  int primary_socket;
  struct sockaddr_in sa;
  primary_socket = Socket(PF_INET, SOCK_STREAM, 0);
  sa = prepareSocketAddress(sa, SERVER_PORT_NUMBER);
  Bind(primary_socket, (struct sockaddr *)&sa, sizeof sa);
  fork_server(primary_socket, sa);
  return 0;   /* no return code implemented */
}


