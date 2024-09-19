#include <winsock2.h>
#include <winsock.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define RESPONSE_BUF_SIZE 1024

int handle_error(char* error_msg, int error_code, SOCKET socket);

int main() {
    // initializing WSAStartup to use winsock
    WORD verreq = MAKEWORD(2, 2);
    WSADATA wsadata;
    int wsa_error = WSAStartup(verreq, &wsadata);
    if (wsa_error != 0) {
        return handle_error("Error during WSAStartup", wsa_error, 0);
    }

    // creating a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return handle_error("Error during socket creation", WSAGetLastError(), sock);
    }

    // connecting the socket
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr= inet_addr("188.114.96.3");
    clientService.sin_port = htons(80);
    int conn_err = connect(sock, (SOCKADDR*) &clientService, sizeof(clientService));
    if (conn_err != 0) {
        return handle_error("Error during socket connection", WSAGetLastError(), sock);
    }

    // send http request
    char request[] = "GET /users HTTP/1.1\nHost: jsonplaceholder.typicode.com\nConnection: close\n\n";
    size_t request_length = sizeof(request);

    DWORD sendBytes;
    WSABUF requestBuff;
    requestBuff.len = request_length;
    requestBuff.buf = (unsigned char*)request;
    int wsa_send_err = WSASend(sock, &requestBuff, 1, &sendBytes, 0, NULL, NULL);
    if (wsa_send_err != 0) {
        return handle_error("Error during WSASend", wsa_send_err, sock);
    }
    
    // recieve http response
    int wsarecv_err;
    unsigned char respByteArr[RESPONSE_BUF_SIZE];
    DWORD recvBytes, flags;
    WSABUF responseBuff;
    responseBuff.len = RESPONSE_BUF_SIZE;
    responseBuff.buf = (char*)respByteArr;

    while (true) {
        wsarecv_err = WSARecv(sock, &responseBuff, 1, &recvBytes, &flags, NULL, NULL);
        if (wsarecv_err != 0) {
            return handle_error("Error during WSARecv", WSAGetLastError(), sock);
        }

        printf("%s\n", responseBuff.buf);

        if (recvBytes == 0) {
            break;
        }
    }

    // cleaning and closing the socket
    closesocket(sock);
    WSACleanup();
    return 0;
}

int handle_error(char* error_msg, int error_code, SOCKET socket) {
    printf("%s, code: %d", error_msg, error_code);
    if (socket != 0) {
        closesocket(socket);
    }
    WSACleanup();
    return 1;
}