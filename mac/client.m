#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <CoreGraphics/CoreGraphics.h>
#include <Foundation/Foundation.h>

int main(int argc, char *argv[])
{
	CGEventRef event;
	CGPoint mouseLocation;
	CGDirectDisplayID mainDisplayId;
	unsigned int width = 0, height = 0;

    width = CGDisplayPixelsWide(mainDisplayId);
    height = CGDisplayPixelsHigh(mainDisplayId);
	printf("w: %d, h: %d\n", width, height);

	struct addrinfo hints, *res;
	int sockfd, status;
	int len, bytes_sent;

	char buffer[1024];
	char buf[1024];
	char *msg = "Prout max here !";

	memset(&hints, 0, sizeof(hints));
	memset(buffer, 0, 1024);

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

  	status = getaddrinfo("192.168.0.104", "3490", &hints, &res);
//  	status = getaddrinfo(NULL, "3490", &hints, &res);
	if (status != 0) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); return 2; }

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) { printf("Error socket\n"); return 2; }
	status = connect(sockfd, res->ai_addr, res->ai_addrlen); if (status != 0) { printf("Couldn't connect\n"); } else printf("Connected\n");

//	bytes_sent = recv(sockfd, buffer, 13, 0); if (bytes_sent < 0) { printf("Couldn't send: %d bytes returned\n", bytes_sent); return 2;}
	printf("Salut\n");
	while (1)
	{
		event = CGEventCreate(NULL);
		mouseLocation = CGEventGetLocation(event);
// 		printf("x: %f, y: %f\n", mouseLocation.x, mouseLocation.y);
		sprintf(buffer, "%f %f", mouseLocation.x, mouseLocation.y);
 		bytes_sent = send(sockfd, buffer, 1024, 0); if (bytes_sent < 0) { printf("Couldn't send: %d bytes returned\n", bytes_sent); return 2;}
		CFRelease(event);
		bytes_sent = recv(sockfd, buf, 1024, 0);
		if (bytes_sent < 0)
		{
			printf("Error bytes_received: %d\n", bytes_sent);
			exit(2);
		}
		while (bytes_sent == 0)
			bytes_sent = recv(sockfd, buf, 1024, 0);
// 		printf("Server answer: %s\n", buf);
	}

	freeaddrinfo(res);
	close(sockfd);
	return 0;
}
