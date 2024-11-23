#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 9090
#define MAX_JOGADORES 4
#define BUFFER_TAM 1024

typedef struct {
    SOCKET socket;
    int id;
    int ativo;
    int pronto_para_jogar;
} Jogador;

Jogador jogadores[MAX_JOGADORES] = {0}; // Lista de jogadores conectados
SOCKET servidorSocket;

void conectar_jogador(SOCKET novoSocket, int jogadorID) {
    char buffer[BUFFER_TAM];

    jogadores[jogadorID].socket = novoSocket;
    jogadores[jogadorID].id = jogadorID + 1;
    jogadores[jogadorID].ativo = 1;
    jogadores[jogadorID].pronto_para_jogar = 0;

    printf("Jogador %d conectado.\n", jogadores[jogadorID].id);

    // Enviar mensagem de boas-vindas ao novo jogador
    sprintf(buffer, "Bem-vindo, Jogador %d!\n", jogadores[jogadorID].id);
    if (send(jogadores[jogadorID].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR) {
        printf("Erro ao enviar mensagem de boas-vindas para Jogador %d. Codigo: %d\n", jogadores[jogadorID].id, WSAGetLastError());
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
                printf("Erro ao enviar notificacao de desconexao para Jogador %d. Codigo: %d\n", jogadores[i].id, WSAGetLastError());
            }
        }
    }

    // Fechar o socket do jogador e marcar como inativo
    closesocket(jogadores[jogadorID].socket);
    jogadores[jogadorID].ativo = 0;
    jogadores[jogadorID].pronto_para_jogar = 0;
}

void verificar_jogadores_prontos() {
    int prontos = 0;

    // Conta quantos jogadores estão prontos
    for (int i = 0; i < MAX_JOGADORES; i++) {
        if (jogadores[i].ativo && jogadores[i].pronto_para_jogar) {
            prontos++;
        }
    }

    // Só começa o jogo se houver pelo menos 2 jogadores prontos
    if (prontos >= 2) {
        // Escolhe uma das 3 opções aleatoriamente
        const char *opcoes[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de Às"};
        srand(time(NULL));
        int escolha = rand() % 3;

        printf("Opção escolhida: %s\n", opcoes[escolha]);

        // Envia comando para iniciar o jogo e a opção escolhida para todos os jogadores
        char mensagem[BUFFER_TAM];
        sprintf(mensagem, "INICIAR_JOGO:%s", opcoes[escolha]);

        for (int i = 0; i < MAX_JOGADORES; i++) {
            if (jogadores[i].ativo) {
                if (send(jogadores[i].socket, mensagem, strlen(mensagem), 0) == SOCKET_ERROR) {
                    printf("Erro ao enviar comando de inicio para Jogador %d. Codigo: %d\n", jogadores[i].id, WSAGetLastError());
                } else {
                    printf("Mensagem de início enviada para Jogador %d.\n", jogadores[i].id);
                }
            }
        }
    } else {
        printf("Esperando mais jogadores. Jogadores prontos: %d\n", prontos);
    }
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
            printf("Erro no select. Codigo: %d\n", WSAGetLastError());
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
                printf("Conexao recusada. Numero maximo de jogadores atingido.\n");
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

                    // Verifica se o jogador está pronto para jogar
                    if (strcmp(buffer, "PRONTO_PARA_JOGAR") == 0) {
                        jogadores[i].pronto_para_jogar = 1;
                        printf("Jogador %d está pronto para jogar.\n", jogadores[i].id);
                        verificar_jogadores_prontos();
                    }
                }
            }
        }
    }
}

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erro ao inicializar Winsock. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == INVALID_SOCKET) {
        printf("Erro ao criar o socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    if (bind(servidorSocket, (struct sockaddr *)&endereco, sizeof(endereco)) == SOCKET_ERROR) {
        printf("Erro no bind. Codigo: %d\n", WSAGetLastError());
        closesocket(servidorSocket);
        WSACleanup();
        return 1;
    }

    if (listen(servidorSocket, MAX_JOGADORES) == SOCKET_ERROR) {
        printf("Erro no listen. Codigo: %d\n", WSAGetLastError());
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
