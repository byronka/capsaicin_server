CFLAGS=-DDEBUG -std=c11 -Wall -std=c11 -pedantic-errors
CC=clang

all: capsaicin_server

capsaicin_server: capsaicin_server.o ntwk_utils.o basic_io.o main.o
	 $(CC) $(CFLAGS) basic_io.o capsaicin_server.o ntwk_utils.o main.o -o capsaicin_server

capsaicin_server.o: capsaicin_server.c
	 $(CC) $(CFLAGS) -c capsaicin_server.c

main.o: main.c
	 $(CC) $(CFLAGS) -c main.c

ntwk_utils.o: ntwk_utils.c
	 $(CC) $(CFLAGS) -c ntwk_utils.c

basic_io.o: basic_io.c
	 $(CC) $(CFLAGS) -c basic_io.c
clean:
	 rm -fr *.o capsaicin_server

servertest: server_tester.o
	 $(CC) $(CFLAGS) server_tester.c -o servertest
