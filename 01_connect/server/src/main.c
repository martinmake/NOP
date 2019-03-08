/*
 * This program creates and listens on a TCP/IP SOCKET
 * for one connection and then prints
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "helpers.h"

int main(void)
{
	// socket file descriptors
	int server_fd, connection_fd;
	// address struct (sockaddr_in == InterNet socket address type) (sockaddr_in6 == IPv6)
	struct sockaddr_in address = { 0 };
	// value that is set to SO in setsockopt
	int opt = 1;
	int adrrlen = sizeof(address);

	// creating server socket file descriptor
	//   (internet)AddressFamily, (Layer3)TCP, (Layer4)IP
	if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		FATAL_ERROR("Socket creation");

	// setsockopt() sets options(SO) on protocol_level(SOL) for socket file descriptor to the value pointed to be opt
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		FATAL_ERROR("Socket set options");

	// set up server address so it accepts connections from any address to for port PORT and set AddressFamily to Internet
	address.sin_family      = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port        = htons(PORT);

	// bind server socket to specified address
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) > 0)
		FATAL_ERROR("Socket bind");

	//                  backLog (maximum queue for pending connections for accept)
	if (listen(server_fd, 0) > 0)
		FATAL_ERROR("Socket listen");

	puts("Waiting for connection...");

	if ((connection_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &adrrlen)) < 0)
		FATAL_ERROR("Socket accept");

	puts("[+] Connection accepted!");

	close(server_fd);

	return 0;
}
