#include <stdio.h>

void
test_foo() {
	int result = createSocket(1);
	if (result != 5) {
		printf("should have been 5 but was %d\n", result);
	}
}

void
test_bar() {
	int result = createSocket(1);
	if (result != 8) {
		printf("should have been 8 but was %d\n", result);
	}
}

void
all_tests() {
	test_foo();
	test_bar();
}

int
main(int argc, char** argv) {
	all_tests();
	return 0;
}
