#include <stdio.h>

void
createSocket_test() {
	int result = createSocket(1);
	if (result < 0) {
		printf("result should have been greater than 0 but was %d\n", result);
	}
}

//void
//prepareSocketAddress_test() {
//	struct sockaddr_in sa;
//	int result = createSocket(1);
//	sa = prepareSocketAddress(sa, 80)
//	printf("\n%d");
//}

void
doBind_test() {

}

int
main(int argc, char** argv) {
	createSocket_test();
	return 0;
}
