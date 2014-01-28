/*
CtlMsg - we parse the message, then return a pointer to a struct
with the info we want to use.
*/

extern struct CtlMsg {
  int sid; /* session id */
  unsigned char action; /* action to undertake - get, put, etc */
  long eid; /* entity id */
  long uid; /* user id */
  long offset; /* where in the file to start pulling from */
  int sbytes; /* number of bytes to send - 1024 by default */
}

extern int
