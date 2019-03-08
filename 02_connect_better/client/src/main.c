#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "helpers.h"

int main(void)
{
	int connection_fd;

	struct addrinfo  hints;
	struct addrinfo *server_info;
	int status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo("localhost", PORT, &hints, &server_info);
	if (status != 0)
		NAME_LOOKUP_ERROR(status);

	connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (connection_fd < 0)
		FATAL_ERROR("Socket create");

	if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) != 0)
		FATAL_ERROR("Socket connect");

	puts("[+] Connection successful!");

	return 0;
}
