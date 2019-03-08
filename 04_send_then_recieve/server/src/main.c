#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include "project_defs.h"
#include "server_defs.h"
#include "functions.h"

int main(void)
{
	int server_fd, connection_fd;

	struct addrinfo  hints;
	struct addrinfo *server_info;
	int status;
	int opt = 1;

	char buf[BUF_SIZE];
	FILE *command_out;
	int   command_exit_status;
	ssize_t bytes_sent;
	ssize_t bytes_recieved;
	uint8_t exit_flag = 0;

	explicit_bzero(&hints, sizeof(hints));
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

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
		FATAL_ERROR("Socket bind");
	puts("[+] Socket options set!");

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
	puts("[+] Server socket closed!");

	while (!exit_flag) {
		explicit_bzero(buf, BUF_SIZE);
		bytes_recieved = recv(connection_fd, buf, BUF_SIZE, 0);
		if (bytes_recieved == -1)
			FATAL_ERROR("Buffer recieve");
		printf("[+] Buffer recieved! (%uB)\n", strlen(buf));

		if (strcmp(buf, EXIT_COMMAND) == 0) {
			exit_flag = 1;
			strcpy(buf, EXIT_STRING);
		} else if (is_str_command(buf, "cd")) {
			if (chdir(buf + 3) != 0)
				strcpy(buf, "Invalid path!");
		} else {
			command_out = popen(buf, "r");
			explicit_bzero(buf, BUF_SIZE);
			read_to_eof(buf, BUF_SIZE, command_out);
			command_exit_status = pclose(command_out);
		}
		buf[BUF_SIZE - 1] = EOF;

		bytes_sent = send(connection_fd, buf, BUF_SIZE, 0);
		if (bytes_sent == -1)
			FATAL_ERROR("Buffer send");
		printf("[+] Buffer sent! (%uB)\n", strlen(buf));
	}

	if (close(connection_fd) != 0)
		FATAL_ERROR("Connection socket close");
	puts("[+] Connection socket closed");

	return 0;
}
