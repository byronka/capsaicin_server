all: pip

pip: create_and_prepare_socket.o mywebserver.o
	 cc create_and_prepare_socket.o mywebserver.o -o pip

create_and_prepare_socket.o: create_and_prepare_socket.c
	 cc -c create_and_prepare_socket.c

mywebserver.o: mywebserver.c
	 cc -c mywebserver.c

clean:
	 rm -fr *.o pip runtests

test: create_and_prepare_socket.o create_and_prepare_socket_tests.o
	 cc create_and_prepare_socket.o create_and_prepare_socket_tests.o -o runtests

create_and_prepare_socket_tests.o: create_and_prepare_socket_tests.c
	 cc -c create_and_prepare_socket_tests.c
	 
