#include <winsock2.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define RESPONSE_BUF_SIZE 4096

int handle_error(const char* error_msg, int error_code, SOCKET socket, FILE* file);

int main() {
    // initializing WSAStartup to use winsock
    WORD verreq = MAKEWORD(2, 2);
    WSADATA wsadata;
    int wsa_error = WSAStartup(verreq, &wsadata);
    if (wsa_error != 0) {
        return handle_error("Error during WSAStartup", wsa_error, 0, NULL);
    }

    // creating a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        return handle_error("Error during socket creation", WSAGetLastError(), sock, NULL);
    }

    // connecting the socket
    struct sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr= inet_addr("188.114.96.3");
    clientService.sin_port = htons(80);
    int conn_err = connect(sock, (SOCKADDR*) &clientService, sizeof(clientService));
    if (conn_err != 0) {
        return handle_error("Error during socket connection", WSAGetLastError(), sock, NULL);
    }

    // send http request
    char request[] = "GET /users HTTP/1.1\nHost: jsonplaceholder.typicode.com\nConnection: close\n\n";
    size_t request_length = sizeof(request);

    DWORD sendBytes;
    DWORD flagsSend = 0;
    WSABUF requestBuff;
    requestBuff.len = request_length;
    requestBuff.buf = (unsigned char*)request;
    int wsa_send_err = WSASend(sock, &requestBuff, 1, &sendBytes, flagsSend, NULL, NULL);
    if (wsa_send_err != 0) {
        return handle_error("Error during WSASend", wsa_send_err, sock, NULL);
    }
    if (sendBytes != request_length) {
        return handle_error("Request was not send fully", 1, sock, NULL);
    }
    
    // prepare to save data to a file
    FILE *file;
    file = fopen("data.txt", "wb");
    if (file == NULL) {
        return handle_error("Error during opening file", 1, sock, file);
    }

    // recieve http response
    unsigned char respByteArr[RESPONSE_BUF_SIZE];
    DWORD recvBytes;
    DWORD flagsRecv = 0;
    WSABUF responseBuff;
    responseBuff.len = RESPONSE_BUF_SIZE;
    responseBuff.buf = respByteArr;

    while (true) {
        int wsarecv_err = WSARecv(sock, &responseBuff, 1, &recvBytes, &flagsRecv, NULL, NULL);
        if (wsarecv_err != 0) {
            return handle_error("Error during WSARecv", WSAGetLastError(), sock, NULL);
        }

        if (recvBytes == 0) {
            break;
        }
        respByteArr[recvBytes] = '\0';
        int file_error = fputs(respByteArr, file);
        if (file_error == EOF) {
            return handle_error("Error during saving file", file_error, sock, file);
        }
    }

    // cleaning and closing the socket
    closesocket(sock);
    WSACleanup();
    fclose(file);
    return 0;
}

int handle_error(const char* error_msg, int error_code, SOCKET socket, FILE* file) {
    printf("%s, code: %d", error_msg, error_code);
    if (socket != 0) {
        closesocket(socket);
    }
    if (file != NULL) {
        fclose(file);
    }
    WSACleanup();
    return 1;
}