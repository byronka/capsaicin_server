#include <stdio.h>
#include "sendfile_wrapper.h"
#include "basic_io.h"

#ifdef __gnu_linux__
#include <sys/sendfile.h>

extern static ssize_t 
Sendfile(int out_fd, int in_fd, off_t *offset, size_t count) {
  ssize_t result = 0;
  if ((result = sendfile(out_fd, in_fd, offset, count)) < 0) {
    perror("\nsendfile error");
    fprintf(stderr, "sendfile error; inputs out_fd[%d] in_fd[%d] offset[%lld]"
	    "offset[%d] count[%lld] returned:[%d]",
	    out_fd, in_fd, offset, count, result);
    Close(out_fd);
    Close(in_fd);
    return result;
  }

  extern int
    sendfile_wrapper(int fd, int s, off_t offset, size_t nbytes) {
    int result;
    result = Sendfile(fd, s, &offset, nbytes);
    Close(fd);
    Close(s);
    return result;
  }
#endif

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

  extern static int
    Sendfile(int fd, int s, off_t offset, size_t nbytes,
	     struct sf_hdtr *hdtr, off_t *sbytes, int flags) {
    int result;
    if ((result = sendfile(fd, s, offset, nbytes, hdtr, sbytes, flags)) < 0) {
      perror("\nsendfile error");
      fprintf(stderr, "sendfile error; inputs fd[%d] s[%d] offset[%lld]"
	      "nbytes[%d] hdtr[%p] sbytes[%lld] flags[%d]",
	      fd, s, offset, nbytes, (void *)hdtr, *sbytes, flags);
    }
    return result;
  }

  extern int
    sendfile_wrapper(int fd, int s, off_t offset, size_t nbytes) {
    int result;
    result = Sendfile(
		      fd,
		      s,
		      0, /* offset of 0 */
		      nbytes, /* number of bytes, 0 being send whole file */
		      NULL, /* no header needed yet */
		      sbytes, /* we'll get the number of bytes sent */
		      0 /* no flags */
		      );
    Close(fd);
    Close(s);
    return result;

  }

#endif
