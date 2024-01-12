#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

// compiler directive to instruct the linker to automatically link the application with a specific library
#pragma comment(lib,"ws2_32.lib") // Winsock Library

// pre-defined structures of winsock library to represent an IPv4 socket address.
struct sockaddr_in server_addr, client_addr;

// Macro for PORT
#define PORT 1010

// argc and argv[] are used to pass information such as ip addresses and port no.
int main(int argc, char* argv[]) {

//    Initialising Winsock library
    WSADATA wsa;

//    created sockets for server and client for connection
    SOCKET server, client;
    int clientAddress = sizeof(client_addr);

//    printf("\nInitialising Winsock...");

//    WSAStartup initializes winsock library
//    MAKEWORD initializes winsock library of version 2
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
//    printf("Initialised.\n");

//    AF_INET representing the address family for Internet Protocol version 4 (IPv4).
//    SOCK_STREAM specifies a TCP stream-oriented socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        closesocket(client);
        closesocket(server);
        WSACleanup();
        return 1;
    }
//    printf("Socket created.\n");

//    passes IPv4 address to structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

//    binding socket to server structure
    if (bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        return 1;
    }
//    printf("Bind done\n");

//    listening clients
    listen(server, 1);

//    printf("Waiting for incoming connections...\n");

//    accepting clients
    client = accept(server, (struct sockaddr*)&client_addr, &clientAddress);
    if (client == INVALID_SOCKET) {
        printf("accept failed with error code : %d\n", WSAGetLastError());
        return 1;
    }

    FILE* image_fp = fopen("D:/WEB Design/360_F_102517057_4Tedp0gKwCkWwu54kKni0GZ0DqIWe5MY.jpg", "rb");
    if (!image_fp) {
        printf("Failed to open image.....");
    }
//    printf("image file opened.\n");

//  fseek function to move the file position indicator to the end of the file (SEEK_END).
    fseek(image_fp, 0, SEEK_END);

//    ftell is used to determine size of the file.
    long imageSize = ftell(image_fp);

//  fseek function to move the file position indicator to the beginning of the file (SEEK_SET).
    fseek(image_fp, 0, SEEK_SET);

    send(client, (char*)&imageSize, sizeof(imageSize), 0);
//    printf("image size sent.\n");
    char img_data[sizeof(imageSize)];
    size_t bytes;

    while ((bytes = fread(img_data, 1, sizeof(img_data), image_fp)) > 0) {
        send(client, img_data, bytes, 0);
    }

//    closing file
    fclose(image_fp);
    printf("image sent.\n");


    closesocket(client);
    closesocket(server);
    WSACleanup();
    exit(0);
}


