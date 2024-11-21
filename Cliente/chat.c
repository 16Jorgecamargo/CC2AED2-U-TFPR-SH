#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <winsock2.h>
#include "chat.h"

#define BUFFER_TAM 1024

void apagar_tela() {
    system("cls");
}

void desenhar_interface() {
    apagar_tela();
    printf("<- ESC para sair       |-- CHAT --|\n");
    for (int i = 2; i < 10; i++) {
        printf("\n");
    }
    printf("mensagem: ");
}

void modo_chat(SOCKET clienteSocket) {
    char mensagem[BUFFER_TAM];
    desenhar_interface();

    while (1) {
        char c = getch();

        if (c == 27) { // Código ASCII do ESC
            printf("\nSaindo do chat...\n");
            break;
        }

        desenhar_interface();
        //printf("mensagem: ");

        fgets(mensagem, sizeof(mensagem), stdin);
        mensagem[strcspn(mensagem, "\n")] = '\0'; // Remove o '\n'

        if (strlen(mensagem) > 0) {
            if (send(clienteSocket, mensagem, strlen(mensagem), 0) == SOCKET_ERROR) {
                printf("Erro ao enviar mensagem. Código: %d\n", WSAGetLastError());
                break;
            }
        }
    }
}
