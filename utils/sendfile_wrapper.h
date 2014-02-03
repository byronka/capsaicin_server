#pragma once
#include <sys/types.h>
extern int sendfile_wrapper(int fd, int s, off_t offset, size_t nbytes);
