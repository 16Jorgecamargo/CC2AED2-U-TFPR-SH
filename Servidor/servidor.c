#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Vincula a biblioteca Winsock

#define PORTA 9090
#define MAX_JOGADORES 4
#define BUFFER_TAM 1024

typedef struct {
    SOCKET socket;
    int id;
    int ativo;
} Jogador;

Jogador jogadores[MAX_JOGADORES] = {0}; // Lista de jogadores conectados
SOCKET servidorSocket;

void conectar_jogador(SOCKET novoSocket, int jogadorID) {
    char buffer[BUFFER_TAM];

    jogadores[jogadorID].socket = novoSocket;
    jogadores[jogadorID].id = jogadorID + 1;
    jogadores[jogadorID].ativo = 1;

    printf("Jogador %d conectado.\n", jogadores[jogadorID].id);

    // Enviar mensagem de boas-vindas ao novo jogador
    sprintf(buffer, "Bem-vindo, Jogador %d! Você está conectado ao servidor.\n", jogadores[jogadorID].id);
    if (send(jogadores[jogadorID].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        printf("Erro ao enviar mensagem de boas-vindas para Jogador %d. Código: %d\n", jogadores[jogadorID].id, WSAGetLastError());
    }

    // Avisar os outros jogadores sobre o novo jogador
    sprintf(buffer, "Jogador %d entrou no jogo.\n", jogadores[jogadorID].id);
    for (int i = 0; i < MAX_JOGADORES; i++) {
        if (jogadores[i].ativo && i != jogadorID) {
            if (send(jogadores[i].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
                printf("Erro ao enviar notificação para Jogador %d. Código: %d\n", jogadores[i].id, WSAGetLastError());
            }
        }
    }
}

void desconectar_jogador(int jogadorID) {
    char buffer[BUFFER_TAM];

    printf("Jogador %d desconectou.\n", jogadores[jogadorID].id);

    // Avisar os outros jogadores sobre a desconexão
    sprintf(buffer, "Jogador %d desconectou.\n", jogadores[jogadorID].id);
    for (int i = 0; i < MAX_JOGADORES; i++) {
        if (jogadores[i].ativo && i != jogadorID) {
            if (send(jogadores[i].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
                printf("Erro ao enviar notificação de desconexão para Jogador %d. Código: %d\n", jogadores[i].id, WSAGetLastError());
            }
        }
    }

    // Fechar o socket do jogador e marcar como inativo
    closesocket(jogadores[jogadorID].socket);
    jogadores[jogadorID].ativo = 0;
}

void loop_principal() {
    struct sockaddr_in endereco;
    int tamEndereco = sizeof(endereco);
    fd_set leituraSockets;
    int maxSocket;

    while (1) {
        FD_ZERO(&leituraSockets);
        FD_SET(servidorSocket, &leituraSockets);
        maxSocket = servidorSocket;

        for (int i = 0; i < MAX_JOGADORES; i++) {
            if (jogadores[i].ativo) {
                FD_SET(jogadores[i].socket, &leituraSockets);
                if (jogadores[i].socket > maxSocket) {
                    maxSocket = jogadores[i].socket;
                }
            }
        }

        int selectResult = select(maxSocket + 1, &leituraSockets, NULL, NULL, NULL);
        if (selectResult == SOCKET_ERROR) {
            printf("Erro no select. Código: %d\n", WSAGetLastError());
            break;
        }

        // Nova conexão
        if (FD_ISSET(servidorSocket, &leituraSockets)) {
            SOCKET novoSocket = accept(servidorSocket, (struct sockaddr *)&endereco, &tamEndereco);

            int conexoesAtivas = 0;
            for (int i = 0; i < MAX_JOGADORES; i++) {
                if (jogadores[i].ativo) {
                    conexoesAtivas++;
                }
            }

            if (conexoesAtivas >= MAX_JOGADORES) {
                char *mensagem = "Servidor cheio. Tente novamente mais tarde.\n";
                send(novoSocket, mensagem, strlen(mensagem), 0);
                closesocket(novoSocket);
                printf("Conexão recusada. Número máximo de jogadores atingido.\n");
            } else {
                for (int i = 0; i < MAX_JOGADORES; i++) {
                    if (!jogadores[i].ativo) {
                        conectar_jogador(novoSocket, i);
                        break;
                    }
                }
            }
        }

        // Processar mensagens dos jogadores conectados
        for (int i = 0; i < MAX_JOGADORES; i++) {
            if (jogadores[i].ativo && FD_ISSET(jogadores[i].socket, &leituraSockets)) {
                char buffer[BUFFER_TAM] = {0};
                int resultado = recv(jogadores[i].socket, buffer, sizeof(buffer) - 1, 0);

                if (resultado <= 0) {
                    desconectar_jogador(i);
                } else {
                    buffer[resultado] = '\0';
                    printf("Jogador %d: %s\n", jogadores[i].id, buffer);
                }
            }
        }
    }
}

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erro ao inicializar Winsock. Código: %d\n", WSAGetLastError());
        return 1;
    }

    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == INVALID_SOCKET) {
        printf("Erro ao criar o socket. Código: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    if (bind(servidorSocket, (struct sockaddr *)&endereco, sizeof(endereco)) == SOCKET_ERROR) {
        printf("Erro no bind. Código: %d\n", WSAGetLastError());
        closesocket(servidorSocket);
        WSACleanup();
        return 1;
    }

    if (listen(servidorSocket, MAX_JOGADORES) == SOCKET_ERROR) {
        printf("Erro no listen. Código: %d\n", WSAGetLastError());
        closesocket(servidorSocket);
        WSACleanup();
        return 1;
    }

    printf("Servidor aguardando jogadores na porta %d...\n", PORTA);
    loop_principal();

    closesocket(servidorSocket);
    WSACleanup();
    return 0;
}
