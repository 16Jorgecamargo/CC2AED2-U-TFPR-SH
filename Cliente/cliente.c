#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#include "menu.h"   // Inclui o cabeçalho do menu
#include "jogar.h"  // Inclui o cabeçalho do jogo

#define PORTA 9090
#define ENDERECO "192.168.1.3" // Atualize para o IP do servidor
#define BUFFER_TAM 1024

void conectar_ao_servidor(SOCKET *clienteSocket) {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erro ao inicializar Winsock. Codigo: %d\n", WSAGetLastError());
        exit(1);
    }

    *clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*clienteSocket == INVALID_SOCKET) {
        printf("Erro ao criar socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    struct sockaddr_in enderecoServidor;
    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(PORTA);
    enderecoServidor.sin_addr.s_addr = inet_addr(ENDERECO);

    if (connect(*clienteSocket, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor)) < 0) {
        printf("Erro ao conectar ao servidor.\n");
        closesocket(*clienteSocket);
        WSACleanup();
        exit(1);
    }

    printf("Conectado ao servidor!\n");
}

int main() {
    SOCKET clienteSocket;
    conectar_ao_servidor(&clienteSocket);

    char listaMenu[4][40] = {"Jogar", "Chat", "Info", "Sair"};
    int opcao;

    // Ciclo principal do cliente
    while (1) {
        // Exibe o menu
        opcao = menu(10, 10, 4, listaMenu);
        if (opcao == 1) { // Jogar
            char mensagemPronto[BUFFER_TAM] = "PRONTO_PARA_JOGAR";
            send(clienteSocket, mensagemPronto, strlen(mensagemPronto), 0);
            printf("Esperando todos os jogadores...\n");

            // Espera até receber o comando de iniciar o jogo do servidor
            while (1) {
                char buffer[BUFFER_TAM] = {0};
                int bytesRecebidos = recv(clienteSocket, buffer, BUFFER_TAM - 1, 0);
                if (bytesRecebidos > 0) {
                    buffer[bytesRecebidos] = '\0';
                    if (strstr(buffer, "INICIAR_JOGO") != NULL) {
                        system("cls"); // Limpa o menu da tela antes de iniciar o jogo
                        iniciar_jogo(clienteSocket);
                        break;
                    }
                }
            }
        } else if (opcao == 4) { // Sair
            break;
        }
    }

    closesocket(clienteSocket);
    WSACleanup();
    return 0;
}
