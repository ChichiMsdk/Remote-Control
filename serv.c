#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <sys/types.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"ws2_32.lib")

#define PORT 3490
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET server_fd, new_socket;
	int valread = 0;
	int valsent = 0;
	float x = 0;
	float y = 0;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{ 
		printf("Failed. Error Code : %d",WSAGetLastError()); 
		return 1;
	}
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
	{ 
		printf("Could not create socket : %d" , WSAGetLastError());
	}

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	printf("Waiting connection..\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	printf("Waiting bytes to receive..\n");
    while (1) {
        valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
		int tmp = valread;
 		buffer[tmp] = 0;
		if (valread > 0)
		{
// 			sscanf(buffer, "%d,%d", &x, &y);
			sscanf_s(buffer, "%f %f", &x, &y);
			SetCursorPos((int)x, (int)y);
			valsent = send(new_socket, "OK", 3, 0);
			if (valsent != 3)
			{
				printf("Error valsent: %d\n", valsent);
				exit(2);
			}
		}
    }
	closesocket(new_socket);
	closesocket(server_fd);
    return 0;
} 
