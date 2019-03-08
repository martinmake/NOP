#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "helpers.h"

int main(void)
{
	int connection_fd;

	struct addrinfo  hints;
	struct addrinfo *server_info;
	int status;

	char buf[1024];
	ssize_t bytes_recv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo("localhost", PORT, &hints, &server_info);
	if (status != 0)
		NAME_RESOLUTION_ERROR(status);
	puts("[+] Name resolution success!");

	connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (connection_fd < 0)
		FATAL_ERROR("Socket create");
	puts("[+] Socket created!");

	puts("[*] Connecting...");
	if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) != 0)
		FATAL_ERROR("Socket connect");
	puts("[+] Connection successful!");

	bytes_recv = recv(connection_fd, buf, sizeof(buf), 0);
	if (bytes_recv == -1)
		FATAL_ERROR("Buffer recieve");
	puts("[+] Buffer recieved!");

	puts("# COMMUNICATION_START #");
	printf("server: %s\n", buf);
	puts("#  COMMUNICATION_END  #");

	if (close(connection_fd) != 0)
		FATAL_ERROR("Socket close");
	puts("[+] Connection closed!");

	return 0;
}
