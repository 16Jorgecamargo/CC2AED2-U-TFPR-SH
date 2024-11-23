
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
int menu(int lin1, int col1, int qtd, char lista[3][40]);
void textColor(int letras, int fundo);

// Cores de texto e fundo
enum {
    BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
    DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
};
enum {
    _BLACK = 0, _BLUE = 16, _GREEN = 32, _CYAN = 48, _RED = 64,
    _MAGENTA = 80, _BROWN = 96, _LIGHTGRAY = 112, _DARKGRAY = 128,
    _LIGHTBLUE = 144, _LIGHTGREEN = 160, _LIGHTCYAN = 176,
    _LIGHTRED = 192, _LIGHTMAGENTA = 208, _YELLOW = 224, _WHITE = 240
};

// Função para definir cor do texto
void textColor(int letra, int fundo) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letra + fundo);
}

// Função para definir a posição do cursor
void linhaCol(int lin, int col) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col - 1, lin - 1});

    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Função para desenhar um box na tela
void box(int lin1, int col1, int lin2, int col2) {
    int i, j;

    // Monta o Box
    for (i = col1; i <= col2; i++) {
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for (i = lin1; i <= lin2; i++) {
        linhaCol(i, col1);
        printf("%c", 179);
        linhaCol(i, col2);
        printf("%c", 179);
    }

    for (i = lin1 + 1; i < lin2; i++) {
        for (j = col1 + 1; j < col2; j++) {
            linhaCol(i, j);
            printf(" ");
        }
    }

    // Posição dos cantos
    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}

// Função para exibir o menu e retornar a opção escolhida
int menu(int lin1, int col1, int qtd, char lista[][40]) {
    int opc = 1, lin2, col2, linha, i, tamMaxItem, tecla;

    tamMaxItem = strlen(lista[0]);
    for (i = 1; i < qtd; i++) {
        if (strlen(lista[i]) > tamMaxItem) {
            tamMaxItem = strlen(lista[i]);
        }
    }
    lin2 = lin1 + (qtd * 2 + 2);
    col2 = col1 + tamMaxItem + 4;

    textColor(WHITE, _BLUE);
    setlocale(LC_ALL, "C");
    box(lin1, col1, lin2, col2);
    setlocale(LC_ALL, "");

    while (1) {
        linha = lin1 + 2;
        for (i = 0; i < qtd; i++) {
            if (i + 1 == opc) {
                textColor(BLACK, _LIGHTGREEN);
            } else {
                textColor(WHITE, _BLUE);
            }
            linhaCol(linha, col1 + 2);
            printf("%s", lista[i]);
            linha += 2;
        }

        linhaCol(1, 1);
        tecla = getch();

        if (tecla == 27) { // ESC para sair
            opc = 0;
            break;
        } else if (tecla == 13) { // ENTER para selecionar
            break;
        } else if (tecla == 72) { // Seta para cima
            if (opc > 1) opc--;
        } else if (tecla == 80) { // Seta para baixo
            if (opc < qtd) opc++;
        }
    }
    return opc;
}
