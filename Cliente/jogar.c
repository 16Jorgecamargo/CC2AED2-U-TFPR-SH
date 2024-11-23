#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "jogar.h"

#define BUFFER_TAM 1024

void mostrar_animacao() {
    const char *opcoes[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de �s"};
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configurar cor do console
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Texto branco no fundo padr�o

    // Anima��o para escolha aleat�ria da mesa
    for (int i = 0; i < 10; i++) {
        // Define a posi��o do cursor para o in�cio da tela (em vez de usar "cls" para limpar)
        COORD coord = {0, 0};
        SetConsoleCursorPosition(consoleHandle, coord);

        // Exibe a op��o aleat�ria
        printf("Escolhendo mesa...\n");
        printf("%s\n", opcoes[i % 3]);

        // For�a o console a atualizar imediatamente
        fflush(stdout);

        // Pausa para criar o efeito da anima��o
        Sleep(200);
    }
}

void iniciar_jogo(SOCKET clienteSocket) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configura a cor do texto para branco no fundo padr�o
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Limpa a tela para come�ar o jogo
    system("cls");

    // Recebe a op��o escolhida do servidor
    char escolha[BUFFER_TAM] = {0};
    int bytesRecebidos = recv(clienteSocket, escolha, BUFFER_TAM - 1, 0);
    if (bytesRecebidos <= 0) {
        printf("Erro ao receber mensagem do servidor. Encerrando o jogo.\n");
        return;
    }

    escolha[bytesRecebidos] = '\0';

    // Extrai a op��o da mensagem recebida
    char *opcaoEscolhida = strstr(escolha, "INICIAR_JOGO:");
    if (opcaoEscolhida != NULL) {
        opcaoEscolhida += strlen("INICIAR_JOGO:"); // Avan�a o ponteiro para a parte da op��o
    } else {
        opcaoEscolhida = "Desconhecido";
    }

    // Limpa a tela para mostrar a anima��o de escolha da mesa
    system("cls");

    // Mostra a anima��o de escolha da mesa
    mostrar_animacao();

    // Exibe a escolha final
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);  // Cor verde para destacar a escolha final
    system("cls");
    printf("Op��o escolhida: %s\n", opcaoEscolhida);

    // Mensagem de in�cio do jogo
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Texto branco no fundo padr�o
    printf("\nJogo iniciado! Bem-vindo ao jogo.\n");
    printf("Hello World!\n");

    // Loop do jogo - esse bloco pode ser substitu�do pela l�gica do jogo real
    while (1) {
        // Implementar a l�gica do jogo aqui
        Sleep(100); // Para n�o sobrecarregar a CPU em um loop infinito
    }
}
