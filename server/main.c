#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

// compiler directive to instruct the linker to automatically link the application with a specific library
#pragma comment(lib,"ws2_32.lib") // Winsock Library

// pre-defined structures of winsock library to represent an IPv4 socket address.
struct sockaddr_in server_addr, client_addr;

// Macro for PORT
#define PORT 9090

// DWORD WINAPI is used for windows programming
// DWORD ---> double word ---> 32-bit unsigned integer
// WINAPI specifies the standard calling convention for Windows API functions

DWORD WINAPI receiveMessages(void* socket) {
//    Typecasting for socket
    SOCKET client = *(SOCKET*)socket;
    char buffer[1024 + 1];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            printf("Receive error or client disconnected\n");
            break;
        }
        printf("Ali: %s\n", buffer);
    }

    return 0;
}

// argc and argv[] are used to pass information such as ip addresses and port no.
int main(int argc, char* argv[]) {

//    Initialising Winsock library
    WSADATA wsa;

    printf("\nInitialising Winsock...");
//    WSAStartup initializes winsock library
//    MAKEWORD initializes winsock library of version 2
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

//    created sockets for server and client for connection
    SOCKET server, client;
    int clientAddress = sizeof(client_addr);
    char server_msg[1024 + 1] = { 0, };

//    AF_INET representing the address family for Internet Protocol version 4 (IPv4).
//    SOCK_STREAM specifies a TCP stream-oriented socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        closesocket(server);
        closesocket(client);
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

//    passes IPv4 address to structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

//    binding socket to server structure
    if (bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        closesocket(server);
        closesocket(client);
        WSACleanup();
        return 1;
    }
    printf("Bind done\n");

//    listening clients
    listen(server, 5);

    printf("Waiting for incoming connections...\n");

//    accepting clients
    client = accept(server, (struct sockaddr*)&client_addr, &clientAddress);
    if (client == INVALID_SOCKET) {
        printf("accept failed with error code : %d\n", WSAGetLastError());
        closesocket(server);
        closesocket(client);
        WSACleanup();
        return 1;
    }
    printf("Connection accepted\n");

    // Change text color to bright cyan
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // Display "Chat application" heading
    printf("Chat application\n");

    // Reset text color to default
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);


    // Chat
    printf("Start chat (type 'exit' to stop):\n");

//    creating thread
//    Thread is used for receiving messages which is running in parallel
    HANDLE recvThread;
    recvThread = CreateThread(NULL, 0, receiveMessages, (void*)&client, 0, NULL);
    if (recvThread == NULL) {
        printf("CreateThread failed with error code : %d\n", GetLastError());
        CloseHandle(recvThread);
        closesocket(server);
        closesocket(client);
        WSACleanup();
        return 1;
    }

    while (1) {
//        taking user input
            fgets(server_msg, 1024, stdin);
            server_msg[strcspn(server_msg, "\n")] = 0; // Remove newline character

        if (strcmp(server_msg, "exit") == 0) {

            break;

        }
        else if(strcmp(server_msg,"send_image") == 0){

            system("\"D:\\1st semester project\\serverSendIMG\\cmake-build-debug\\serverSendIMG.exe\"");

        }
        else if(strcmp(server_msg,"receive_image") == 0){

            system("\"D:\\1st semester project\\IMGrecv\\cmake-build-debug\\IMGrecv.exe\"");

        }
        else {

            send(client, server_msg, strlen(server_msg), 0);

        }
    }

    WaitForSingleObject(recvThread, INFINITE);
    CloseHandle(recvThread);
    closesocket(server);
    closesocket(client);
    WSACleanup();

    return 0;
}