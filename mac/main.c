#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>

// TODO: profiler check time to update mouse pos

int main(int argc, char *argv[])
{
	struct addrinfo hints = {0}; struct addrinfo *res = {0}; struct addrinfo *p;
	struct sockaddr_storage their_addr;
	socklen_t addr_size;
	int sockfd, newfd, iResult; int status; int bytes_received;
	char ipstr[INET6_ADDRSTRLEN]; char buff[512]; 

	memset(buff, 0, 512); memset(&hints, 0, sizeof(hints));
	addr_size = sizeof(their_addr); bytes_received = 0;

	hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; hints.ai_protocol = 0;

	status = getaddrinfo("127.0.0.1", "3490", &hints, &res);
	if (status != 0) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); return 2; }

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) { printf("Error socket\n"); return 2; }

	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) { printf("Bind error\n"); return 2; }
	if (listen(sockfd, 10) < 0) { printf("Listen error\n"); return 2; }

	newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	if (newfd < 0) { printf("Error accept\n"); return 2; }

	do
	{
		bytes_received = recv(newfd, buff, 512, MSG_WAITALL);
		printf("bytes_received: %d\n", bytes_received);
	} while(bytes_received > 0);
	if (bytes_received < 0) { fprintf(stderr, "Error with recv\n"); }

	printf("buff: %s\n", buff);

	close(sockfd); close(newfd);
	freeaddrinfo(res); // free the linked list

	return 0;
}
