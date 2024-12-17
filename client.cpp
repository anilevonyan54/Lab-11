#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA windowsSocketData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddress;
    char messageBuffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &windowsSocketData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        perror("Failed to connect to server");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    printf("Successfully connected to the server.\n");

    while (1) {
        printf("Type your message: ");
        fgets(messageBuffer, BUFFER_SIZE, stdin);

        send(clientSocket, messageBuffer, strlen(messageBuffer), 0);

        memset(messageBuffer, 0, BUFFER_SIZE);

        int receivedBytes = recv(clientSocket, messageBuffer, BUFFER_SIZE, 0);
        if (receivedBytes <= 0) {
            printf("Server has disconnected.\n");
            break;
        }

        printf("Message from server: %s\n", messageBuffer);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
