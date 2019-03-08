#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "helpers.h"

int main(void)
{
	int server_fd, connection_fd;

	struct addrinfo  hints;
	struct addrinfo *server_info;
	int status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo("localhost", PORT, &hints, &server_info);
	if (status != 0)
		NAME_LOOKUP_ERROR(status);

	server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (server_fd < 0)
		FATAL_ERROR("Socket create");

	if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) != 0)
		FATAL_ERROR("Socket bind");

	if (listen(server_fd, 0) != 0)
		FATAL_ERROR("Socket listen");

	puts("Waiting for connection...");

	connection_fd = accept(server_fd, server_info->ai_addr, (socklen_t *) &server_info->ai_addrlen);
	if (connection_fd < 0)
		FATAL_ERROR("Socket create");

	puts("[+] Connection accepted!");

	return 0;
}
