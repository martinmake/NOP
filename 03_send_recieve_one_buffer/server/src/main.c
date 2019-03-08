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

	char buf[1024] = "Greetings from  server!";
	ssize_t bytes_sent;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo("localhost", PORT, &hints, &server_info);
	if (status != 0)
		NAME_RESOLUTION_ERROR(status);
	puts("[+] Name resolution success!");

	server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (server_fd < 0)
		FATAL_ERROR("Socket create");
	puts("[+] Socket created!");

	if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) != 0)
		FATAL_ERROR("Socket bind");
	puts("[+] Socket is bound to port: "PORT"!");

	if (listen(server_fd, 0) != 0)
		FATAL_ERROR("Socket listen");
	puts("[+] Socket is listening!");

	puts("[*] Waiting for connection...");
	connection_fd = accept(server_fd, server_info->ai_addr, (socklen_t *) &server_info->ai_addrlen);
	if (connection_fd < 0)
		FATAL_ERROR("Socket create");
	puts("[+] Connection accepted!");

	if (close(server_fd) != 0)
		FATAL_ERROR("Server socket close");
	puts("[+] Server socket closed");

	bytes_sent = send(connection_fd, buf, sizeof(buf), 0);
	if (bytes_sent == -1)
		FATAL_ERROR("Buffer send");
	puts("[+] Buffer sent!");

	if (close(connection_fd) != 0)
		FATAL_ERROR("Connection socket close");
	puts("[+] Connection socket closed");

	return 0;
}
