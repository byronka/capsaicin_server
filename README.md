Important notes:
The system was compiled with gcc 4.8
I used exuberant ctags v. 5.8

The way the system works:

1) There is a Server, whose job is to listen for clients.  It uses the
CtlMsg program to decrypt the message to plaintext, and then stores that 
in the circ_ctlmsg_buf.

2) there is a program called circ_ctlmsg_buf.  It stores requests that 
the Server has given it, in a circular queue.

3) There are multiple programs, Request_handlers.  They subscribe to
the circ_ctlmsg_buf and are made aware of whenever there is an item waiting
in the queue.  As soon as one of them is free, they will try to grab that item.

4) The control message, aka ctlmsg, is a text line described in CtlMsg.c, and
has all the information necessary for the request handler to fulfill the
request.

5) CtlMsg decrypts the message before it is stored in the 
circular buffer.

6) The circ_ctlmsg_buf program is being hit on two sides - by
the server, and by multiple request_handlers.  Both sides
will be slamming it hard, but we don't want it to become a choke point.
It is important to synchronize data access and provide smooth flow - but
primarily, synchronize.

6a) The circ_ctlmsg_buf needs to have design happen at the edges, that is,
handle the situation where the server is talking to it, and also managing
all the connections to various request_handlers.  One socket for the server,
and one per request handler.  each of these need synchronization coding
to prevent clobbering of data.