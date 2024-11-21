#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "chat.h" // Inclui a interface para o chat

#pragma comment(lib, "ws2_32.lib")

#define PORTA 9090
#define ENDERECO "10.0.66.96"
#define BUFFER_TAM 1024

DWORD WINAPI thread_receber(LPVOID param) {
    SOCKET clienteSocket = *(SOCKET*)param;
    char buffer[BUFFER_TAM];

    while (1) {
        memset(buffer, 0, BUFFER_TAM);
        int bytesRecebidos = recv(clienteSocket, buffer, BUFFER_TAM - 1, 0);

        if (bytesRecebidos <= 0) {
            printf("Conexão com o servidor perdida.\n");
            break;
        }

        buffer[bytesRecebidos] = '\0';
        printf("%s", buffer);
    }

    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET clienteSocket;
    struct sockaddr_in enderecoServidor;
    HANDLE threadHandle;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erro ao inicializar Winsock. Código: %d\n", WSAGetLastError());
        return 1;
    }

    clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket == INVALID_SOCKET) {
        printf("Erro ao criar o socket. Código: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(PORTA);
    enderecoServidor.sin_addr.s_addr = inet_addr(ENDERECO);

    if (connect(clienteSocket, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor)) == SOCKET_ERROR) {
        printf("Erro ao conectar ao servidor. Código: %d\n", WSAGetLastError());
        closesocket(clienteSocket);
        WSACleanup();
        return 1;
    }

    printf("Conectado ao servidor na porta %d.\n", PORTA);
    printf("Pressione 't' para abrir o modo chat ou Ctrl+C para sair.\n");

    threadHandle = CreateThread(NULL, 0, thread_receber, &clienteSocket, 0, NULL);
    if (threadHandle == NULL) {
        printf("Erro ao criar thread de recebimento.\n");
        closesocket(clienteSocket);
        WSACleanup();
        return 1;
    }

    while (1) {
        char comando = getch();

        if (comando == 't') {
            modo_chat(clienteSocket); // Chama o chat
            printf("Voltando ao cliente principal...\n");
        }
    }

    closesocket(clienteSocket);
    CloseHandle(threadHandle);
    WSACleanup();

    return 0;
}
