#include <CtlMsg.h>

/*
CtlMsg - we parse the message, then return a pointer to a struct
with the info we want to use.
*/

extern struct CtlMsg {
  int sid; /* session id */
  int action; /* action: get, set, ok, etc... */
  long eid; /* entity id: video id, etc. */
  long uid; /* user id */
  long offset; /* offset into the file */
  int sbytes; /* number of bytes we want */
  int seq_num; /* sequence # of the client / server conversation */
  long timestamp; /* timestamp - avoid replay attacks and other uses */
  int uip; /* user ip addr - i.e. 192.168.56.2 */
}

extern CtlMsg*
string_to_struct(char* msg) {
  //TODO - BK - 2/1/2014 - do I need to include max size of string in args?
  static CtlMsg myCtlMsg;
  return &myCtlMsg;

}
