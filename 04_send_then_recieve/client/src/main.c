#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "project_defs.h"

int main(void)
{
	int connection_fd;

	struct addrinfo  hints;
	struct addrinfo *server_info;
	int gai_status;

	char buf[BUF_SIZE];
	ssize_t bytes_recieved;
	ssize_t bytes_sent;
	size_t  command_len;
	uint8_t exit_flag = 0;

	explicit_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	gai_status = getaddrinfo("localhost", PORT, &hints, &server_info);
	if (gai_status != 0)
		NAME_RESOLUTION_ERROR(gai_status);
	puts("[+] Name resolution success!");

	connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (connection_fd < 0)
		FATAL_ERROR("Socket create");
	puts("[+] Socket created!");

	puts("[*] Connecting...");
	if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) != 0)
		FATAL_ERROR("Socket connect");
	puts("[+] Connection successful!");

	puts("# COMMUNICATION_START #");
	while (!exit_flag) {
		explicit_bzero(buf, BUF_SIZE);
		printf("> ");
		fgets(buf, BUF_SIZE, stdin);

		command_len = strlen(buf);
		if (buf[command_len - 1] == '\n')
			buf[command_len - 1] = '\0';

		if (strcmp(buf, EXIT_COMMAND) == 0)
			exit_flag = 1;

		bytes_sent = send(connection_fd, buf, BUF_SIZE, 0);
		if (bytes_recieved == -1)
			FATAL_ERROR("Buffer recieve");

		explicit_bzero(buf, BUF_SIZE);
		bytes_recieved = recv(connection_fd, buf, BUF_SIZE, 0);
		if (bytes_recieved == -1)
			FATAL_ERROR("Buffer recieve");

		puts(buf);
	}
	puts("#  COMMUNICATION_END  #");

	if (close(connection_fd) != 0)
		FATAL_ERROR("Socket close");
	puts("[+] Connection closed!");

	return 0;
}
