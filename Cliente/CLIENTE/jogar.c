#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "jogar.h"
#include "menu.h"

#define BUFFER_TAM 1024
#define MAX_JOGADORES 4

int mostrar_cartas_menu(int jogador_id, char lista_cartas[][40])
{
    int lin1 = 10, col1 = 10, qtd = 5;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Limpa a tela antes de mostrar o menu
    system("cls");

    // Desenha um cabeçalho
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    linhaCol(8, col1);
    printf("Suas cartas:\n");

    // Mostra todas as cartas primeiro
    for (int i = 0; i < qtd; i++)
    {
        linhaCol(lin1 + i*2, col1);
        printf("%d. %s", i+1, lista_cartas[i]);
    }

    // Aguarda input do usuário
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    linhaCol(lin1 + qtd*2 + 2, col1);
    printf("Escolha uma carta (1-%d): ", qtd);

    int opcao = 0;
    do
    {
        opcao = _getch() - '0';
    }
    while (opcao < 1 || opcao > qtd);

    // Mostra a carta selecionada
    linhaCol(lin1 + qtd*2 + 4, col1);
    printf("Você escolheu a carta: %s", lista_cartas[opcao - 1]);

    // Restaura a cor padrão
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return opcao - 1; // Retorna o índice da carta escolhida
}

void mostrar_animacao_distribuicao()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const char* frames[] =
    {
        "Distribuindo cartas   ",
        "Distribuindo cartas.  ",
        "Distribuindo cartas.. ",
        "Distribuindo cartas..."
    };

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    for (int i = 0; i < 12; i++)
    {
        system("cls");
        printf("\n\n%s", frames[i % 4]);
        Sleep(300);
    }
}

void mostrar_animacao_mesa()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    const char* mesas[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de Aces"};

    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    for (int i = 0; i < 10; i++)
    {
        system("cls");
        printf("\n\nEscolhendo mesa...\n");
        printf("%s", mesas[i % 3]);
        Sleep(300);
    }
}

void iniciar_jogo(SOCKET clienteSocket)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    char buffer[BUFFER_TAM];
    char lista_cartas[5][40];
    const char* mesa_atual = NULL;

    // Limpa a tela e prepara para o início do jogo
    system("cls");
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\n\nIniciando o jogo...\n");
    Sleep(1000);

    while (1)
    {
        memset(buffer, 0, BUFFER_TAM);
        int bytes = recv(clienteSocket, buffer, BUFFER_TAM-1, 0);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';

            if (strncmp(buffer, "MESA_ESCOLHIDA:", 14) == 0)
            {
                // Mostra animação da escolha da mesa
                mostrar_animacao_mesa();

                // Mostra a mesa escolhida
                mesa_atual = buffer + 14;
                system("cls");
                SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("\n\nMesa escolhida: %s\n", mesa_atual);
                Sleep(2000);
            }
            else if (strstr(buffer, "CARTAS:") != NULL)
            {
                // Mostra animação de distribuição
                mostrar_animacao_distribuicao();

                // Processa as cartas recebidas
                char cartas_temp[BUFFER_TAM];
                strcpy(cartas_temp, buffer + 7);  // Pula o "CARTAS:"

                char *token = strtok(cartas_temp, ",");
                int i = 0;
                while (token != NULL && i < 5)
                {
                    strcpy(lista_cartas[i], token);
                    printf("Carta %d: %s\n", i+1, lista_cartas[i]);  // Debug
                    token = strtok(NULL, ",");
                    i++;
                }

                // Limpa a tela e mostra o menu de cartas
                Sleep(1000);  // Pequena pausa para visualização
                system("cls");

                // Recebe a escolha do jogador
                int carta_escolhida = mostrar_cartas_menu(jogador_id, lista_cartas);

                // Prepara e envia a mensagem com a carta escolhida
                char mensagem_escolha[BUFFER_TAM];
                sprintf(mensagem_escolha, "CARTA_ESCOLHIDA:%d:%s", jogador_id, lista_cartas[carta_escolhida]);
                send(clienteSocket, mensagem_escolha, strlen(mensagem_escolha), 0);

                // Aguarda confirmação do servidor
                printf("\nAguardando outros jogadores...\n");
            }
        }
        Sleep(100);
    }

    // Loop do jogo
    int jogo_ativo = 1;
    clock_t inicio = clock();
    double timeout = 60.0; // Timeout de 60 segundos

    while (jogo_ativo)
    {
        // Verifica se o tempo limite foi atingido
        double tempo_decorrido = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        if (tempo_decorrido >= timeout)
        {
            SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            printf("\n\nTempo limite atingido. Encerrando o jogo.\n");
            break;
        }

        // Verifica input do usuário
        if (_kbhit())
        {
            char tecla = _getch();
            if (tecla == 'q' || tecla == 'Q')
            {
                jogo_ativo = 0;
            }
        }
        Sleep(100);
    }

    // Restaura as cores padrão e finaliza
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\nJogo encerrado.\n");
}
