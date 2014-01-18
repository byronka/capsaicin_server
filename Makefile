CFLAGS=-DDEBUG -std=c11 -Wall -std=c11 -pedantic-errors
CC=clang
OBJECTS = capsaicin_server.o ntwk_utils.o basic_io.o main.o

all: capsaicin_server

capsaicin_server: $(OBJECTS)
	 $(CC) $(CFLAGS) $(OBJECTS) -o capsaicin_server

%.o:%.c
	 $(CC) $(CFLAGS) -c -o $@ $<

clean:
	 rm -fr *.o capsaicin_server

servertest: server_tester.o
	 $(CC) $(CFLAGS) server_tester.c -o servertest
