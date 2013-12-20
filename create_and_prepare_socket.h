extern int createSocket();
extern struct sockaddr_in prepareSocketAddress(struct sockaddr_in sa, int port);
extern struct sockaddr_in doBind(int s, struct sockaddr_in sa);
