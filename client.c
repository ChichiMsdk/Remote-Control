#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#pragma comment(lib,"user32.lib")

#define PORT 3490
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char buf[BUFFER_SIZE] = {0};
	int bytes = 0;
	
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }

    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connect failed: %d", WSAGetLastError());
        return 1;
    }

	POINT p;
    while (1)
	{
        GetCursorPos(&p);
		printf("x: %ld, y: %ld\n", p.x, p.y);
        sprintf(buffer, "%ld,%ld", p.x, p.y);
        send(client_socket, buffer, strlen(buffer), 0);
		bytes = recv(client_socket, buf, 1024, 0);
		if (bytes < 0)
		{
			printf("Error bytes received: %d\n", bytes);
			exit(2);
		}
// 		while (bytes == 0)
// 			bytes = recv(client_socket, buf, 1024, 0);
// 		printf("Server answer = %s\n", buf);
// 		Sleep(1000);
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
