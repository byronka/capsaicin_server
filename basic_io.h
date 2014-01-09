#pragma once
#define BACKLOG 4

/***************************************************
**    General IO Routines for the system          **
**                                                **
**Author: Byron Katz, 2014                        **
**Purpose: These methods are used throughout the  **
**  system for general IO to files and sockets.   **
**  Note that methods with capitalized first      **
**  letters are wrapped for error handling.       **
**                                                **
****************************************************/

int Socket(int domain, int type, int protocol);
ssize_t Read(int d, void *buf, size_t nbytes);
ssize_t Write(int d, const void *buf, size_t nbytes);
int Open(const char *path, int flags, ...);
int Close(int fd);
int Sendfile(int fd, int s, off_t offset, size_t nbytes, struct sf_hdtr *hdtr, off_t *sbytes, int flags);
int simple_sendfile(int fd, int s, off_t *sbytes);
int Accept(int s, struct sockaddr * restrict addr, socklen_t * restrict addrlen);
int Listen(int s, int backlog);
int Bind(int s, const struct sockaddr *addr, socklen_t addrlen);
struct sockaddr_in prepareSocketAddress(struct sockaddr_in sa, int port);
