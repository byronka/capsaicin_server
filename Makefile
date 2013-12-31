CFLAGS=-DDEBUG
CC=clang

all: capsaicin_server

capsaicin_server: create_and_prepare_socket.o capsaicin_server.o ntwk_utils.o main.o
	 $(CC) $(CFLAGS) create_and_prepare_socket.o capsaicin_server.o ntwk_utils.o main.o -o capsaicin_server

create_and_prepare_socket.o: create_and_prepare_socket.c
	 $(CC) $(CFLAGS) -c create_and_prepare_socket.c

capsaicin_server.o: capsaicin_server.c
	 $(CC) $(CFLAGS) -c capsaicin_server.c

main.o: main.c
	 $(CC) $(CFLAGS) -c main.c

ntwk_utils.o: ntwk_utils.c
	 $(CC) $(CFLAGS) -c ntwk_utils.c

clean:
	 rm -fr *.o capsaicin_server runtests

test: create_and_prepare_socket.o create_and_prepare_socket_tests.o
	 $(CC) $(CFLAGS) create_and_prepare_socket.o create_and_prepare_socket_tests.o -o runtests
	 ./runtests
	 rm runtests

create_and_prepare_socket_tests.o: create_and_prepare_socket_tests.c
	 $(CC) $(CFLAGS) -c create_and_prepare_socket_tests.c

servertest: server_tester.o
	 $(CC) $(CFLAGS) server_tester.c -o servertest
