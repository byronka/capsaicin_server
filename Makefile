CFLAGS=-gstabs+ -DDEBUG -std=c11 -Wall -pedantic-errors
CC=gcc
OBJECTS = prime_server/capsaicin_server.o utils/ntwk_utils.o \
 utils/basic_io.o prime_server/main.o utils/sendfile_wrapper.o \
 ctlmsg/Ctlmsg.o circ_ctlmsg_buf/circ_ctlmsg_buf.o

all: capsaicin_server

capsaicin_server: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) -o capsaicin_server

%.o:%.c
	 $(CC) $(CFLAGS) -c -o $@ $<

clean:
	 rm -fr *.o capsaicin_server

servertest: server_tester.o
	 $(CC) $(CFLAGS) server_tester.c -o servertest
