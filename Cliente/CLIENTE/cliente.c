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

void conectar_ao_servidor(SOCKET *clienteSocket)
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Erro ao inicializar Winsock. Codigo: %d\n", WSAGetLastError());
        exit(1);
    }

    *clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*clienteSocket == INVALID_SOCKET)
    {
        printf("Erro ao criar socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }

    struct sockaddr_in enderecoServidor;
    enderecoServidor.sin_family = AF_INET;
    enderecoServidor.sin_port = htons(PORTA);
    enderecoServidor.sin_addr.s_addr = inet_addr(ENDERECO);

    if (connect(*clienteSocket, (struct sockaddr *)&enderecoServidor, sizeof(enderecoServidor)) < 0)
    {
        printf("Erro ao conectar ao servidor.\n");
        closesocket(*clienteSocket);
        WSACleanup();
        exit(1);
    }

    printf("Conectado ao servidor!\n");
}

int main()
{
    SOCKET clienteSocket;
    conectar_ao_servidor(&clienteSocket);
    char listaMenu[4][40] = {"Jogar", "Chat", "Info", "Sair"};
    char listaMenuIniciar[1][40] = {"Iniciar Jogo"};
    int opcao;
    int meu_id = -1;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Recebe ID do servidor
    char buffer[BUFFER_TAM] = {0};
    recv(clienteSocket, buffer, BUFFER_TAM - 1, 0);
    if (strstr(buffer, "Bem-vindo, Jogador") != NULL)
    {
        sscanf(buffer, "Bem-vindo, Jogador %d!", &meu_id);
    }

    while (1)
    {
        opcao = menu(10, 10, 4, listaMenu);
        if (opcao == 1)   // Jogar
        {
            // Limpa a tela e mostra feedback imediato
            system("cls");
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("Aguardando outros jogadores...\n");

            char mensagemPronto[BUFFER_TAM] = "PRONTO_PARA_JOGAR";
            send(clienteSocket, mensagemPronto, strlen(mensagemPronto), 0);

            while (1)
            {
                char buffer[BUFFER_TAM] = {0};
                int bytesRecebidos = recv(clienteSocket, buffer, BUFFER_TAM - 1, 0);
                if (bytesRecebidos > 0)
                {
                    buffer[bytesRecebidos] = '\0';

                    if (strstr(buffer, "MOSTRAR_MENU_INICIAR") != NULL)
                    {
                        system("cls");
                        int escolha = menu(10, 10, 1, listaMenuIniciar);
                        if (escolha == 1)
                        {
                            // Feedback visual ao iniciar o jogo
                            system("cls");
                            SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                            printf("Iniciando o jogo...\n");

                            // Envia mensagem de início para o servidor
                            const char* msgIniciar = "INICIAR_JOGO";
                            send(clienteSocket, msgIniciar, strlen(msgIniciar), 0);
                        }
                    }
                    else if (strstr(buffer, "AGUARDAR_JOGADOR1") != NULL)
                    {
                        system("cls");
                        SetConsoleTextAttribute(consoleHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        printf("Aguardando jogador 1 iniciar...\n");
                    }
                    else if (strstr(buffer, "INICIAR_JOGO:") != NULL)
                    {
                        printf("Recebido comando para iniciar jogo\n");  // Debug
                        iniciar_jogo(clienteSocket, meu_id);  // Passa o meu_id como parâmetro
                        break;
                    }
                }
                Sleep(100); // Pequena pausa para não sobrecarregar o CPU
            }
        }
        else if (opcao == 4)   // Sair
        {
            break;
        }
    }

    closesocket(clienteSocket);
    WSACleanup();
    return 0;
}
