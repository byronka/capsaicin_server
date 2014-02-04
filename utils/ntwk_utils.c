#include <netinet/in.h>
#include <stdio.h>

extern void
print_ip_addr_32_bit (unsigned char* client_addr_c) {
	fprintf(stderr, "%u.%u.%u.%u\n", client_addr_c[0],client_addr_c[1],client_addr_c[2],client_addr_c[3]);
}

extern unsigned char*
convert_ip_addr (uint32_t number) {
	static unsigned char finished_version[4];
	finished_version[3] = number % 256;
	finished_version[2] = (number >> 8) % 256;
	finished_version[1] = (number >> 16) % 256;
	finished_version[0] = (number >> 24) % 256;
	return finished_version;
}
