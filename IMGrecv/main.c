#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>

// compiler directive to instruct the linker to automatically link the application with a specific library
#pragma comment(lib,"ws2_32.lib") // Winsock Library

// pre-defined structures of winsock library to represent an IPv4 socket address.
struct sockaddr_in server_addr;

// Macro for PORT
#define PORT 1010

// argc and argv[] are used to pass information such as ip addresses and port no.
int main(int argc, char* argv[]) {

//    created socket for client for connection
    SOCKET client;

//    printf("\nInitialising Winsock...");
    WSADATA wsa;
//    WSAStartup initializes winsock library
//    MAKEWORD initializes winsock library of version 2
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error Code : %d\n", WSAGetLastError());
        return 1;
    }
//    printf("Initialised.\n");

//    AF_INET representing the address family for Internet Protocol version 4 (IPv4).
//    SOCK_STREAM specifies a TCP stream-oriented socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return 1;
    }
//    printf("Socket created.\n");

//    passes IPv4 address to structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("10.7.100.186"); // Replace with server IP
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

//    connect with server
    if (connect(client, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connect error\n");
        return 1;
    }
//    printf("Connected\n");

//    image size received
    long imageSize;
    recv(client, (char*)&imageSize, sizeof(imageSize), 0);
//    printf("image size received %ld\n", imageSize);

// open file to write bytes
    FILE* image_fp = fopen("sample.jpg", "wb");
    if (!image_fp) {
        printf("Failed to open image.....");
        return -1;
    }
//    printf("image file opened.\n");
    char img_data[16000];
    long remaining_bytes = imageSize;
    int bytesRead;

    do{
        bytesRead = recv(client, img_data, sizeof(img_data), 0);
        if (bytesRead <= 0) {
            perror("Error in receiving image data");
            // handle the error, break the loop or return
            break;
        }
        remaining_bytes -= bytesRead;
//        fwrite to write receiving image bytes to already developed file
        size_t written = fwrite(img_data, 1, bytesRead, image_fp);

        if (written != bytesRead) {
            perror("Error writing to file");
            break;
        }

    }while (remaining_bytes > 0);
    printf("image received.\n");
    fclose(image_fp);

//    system call used to open the image on a run time.
    system("sample.jpg");

    closesocket(client);
    WSACleanup();

    exit(0);
}


