all: capsaicin_server

capsaicin_server: create_and_prepare_socket.o capsaicin_server.o main.o
	 cc create_and_prepare_socket.o capsaicin_server.o main.o -o capsaicin_server

create_and_prepare_socket.o: create_and_prepare_socket.c
	 cc -c create_and_prepare_socket.c

capsaicin_server.o: capsaicin_server.c
	 cc -c capsaicin_server.c

main.o: main.c
	 cc -c main.c

clean:
	 rm -fr *.o capsaicin_server runtests

test: create_and_prepare_socket.o create_and_prepare_socket_tests.o
	 cc create_and_prepare_socket.o create_and_prepare_socket_tests.o -o runtests
	 ./runtests
	 rm runtests

create_and_prepare_socket_tests.o: create_and_prepare_socket_tests.c
	 cc -c create_and_prepare_socket_tests.c
	 
