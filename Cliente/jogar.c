#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include "jogar.h"

#define BUFFER_TAM 1024

void mostrar_animacao() {
    const char *opcoes[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de Às"};
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configurar cor do console
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Texto branco no fundo padrão

    // Animação para escolha aleatória da mesa
    for (int i = 0; i < 10; i++) {
        // Define a posição do cursor para o início da tela (em vez de usar "cls" para limpar)
        COORD coord = {0, 0};
        SetConsoleCursorPosition(consoleHandle, coord);

        // Exibe a opção aleatória
        printf("Escolhendo mesa...\n");
        printf("%s\n", opcoes[i % 3]);

        // Força o console a atualizar imediatamente
        fflush(stdout);

        // Pausa para criar o efeito da animação
        Sleep(200);
    }
}

void iniciar_jogo(SOCKET clienteSocket) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Configura a cor do texto para branco no fundo padrão
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // Limpa a tela para começar o jogo
    system("cls");

    // Recebe a opção escolhida do servidor
    char escolha[BUFFER_TAM] = {0};
    int bytesRecebidos = recv(clienteSocket, escolha, BUFFER_TAM - 1, 0);
    if (bytesRecebidos <= 0) {
        printf("Erro ao receber mensagem do servidor. Encerrando o jogo.\n");
        return;
    }

    escolha[bytesRecebidos] = '\0';

    // Extrai a opção da mensagem recebida
    char *opcaoEscolhida = strstr(escolha, "INICIAR_JOGO:");
    if (opcaoEscolhida != NULL) {
        opcaoEscolhida += strlen("INICIAR_JOGO:"); // Avança o ponteiro para a parte da opção
    } else {
        opcaoEscolhida = "Desconhecido";
    }

    // Limpa a tela para mostrar a animação de escolha da mesa
    system("cls");

    // Mostra a animação de escolha da mesa
    mostrar_animacao();

    // Exibe a escolha final
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);  // Cor verde para destacar a escolha final
    system("cls");
    printf("Opção escolhida: %s\n", opcaoEscolhida);

    // Mensagem de início do jogo
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Texto branco no fundo padrão
    printf("\nJogo iniciado! Bem-vindo ao jogo.\n");
    printf("Hello World!\n");

    // Loop do jogo - esse bloco pode ser substituído pela lógica do jogo real
    while (1) {
        // Implementar a lógica do jogo aqui
        Sleep(100); // Para não sobrecarregar a CPU em um loop infinito
    }
}
