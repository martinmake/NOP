/*
 * This program attempts to connect to server
 * using a TCP/IP SOCKET
 *
 * concepts from server source were skipped
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "helpers.h"

int main(void)
{
	int client_fd;
	struct sockaddr_in server_address;

	if ((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		FATAL_ERROR("Socket create");

	server_address.sin_family = AF_INET;
	server_address.sin_port   = htons(PORT);

	// str_ip -> net_ip (internetwork_protocot to network)
	if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr.s_addr) <= 0)
		FATAL_ERROR("IP address parse (invalid address)");

	if (connect(client_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		FATAL_ERROR("Socket connect");

	puts("[+] Connection successful!");

	return 0;
}
