#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "util.h"

/**
 * @brief Implementação das funções utilitárias básicas
 */
int minimo(int a, int b) { return (a < b) ? a : b; }
int maximo(int a, int b) { return (a > b) ? a : b; }

/**
 * @brief Posiciona o cursor e o torna invisível
 */
void linhaCol(int lin, int col)
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){col - 1, lin - 1});
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

/**
 * @brief Desenha uma caixa usando caracteres ASCII estendido
 * @details Utiliza caracteres especiais para cantos e bordas
 */
void box(int lin1, int col1, int lin2, int col2)
{
    int i;

    for (i = col1; i <= col2; i++)
    {
        linhaCol(lin1, i);
        printf("%c", 196);
        linhaCol(lin2, i);
        printf("%c", 196);
    }

    for (i = lin1; i <= lin2; i++)
    {
        linhaCol(i, col1);
        printf("%c", 179);
        linhaCol(i, col2);
        printf("%c", 179);
    }

    linhaCol(lin1, col1);
    printf("%c", 218);
    linhaCol(lin1, col2);
    printf("%c", 191);
    linhaCol(lin2, col1);
    printf("%c", 192);
    linhaCol(lin2, col2);
    printf("%c", 217);
}

/**
 * @brief Limpa a tela e exibe mensagem opcional
 * @details Implementação específica para Windows
 */
void limparTela(const char *mensagem)
{
    system("cls");
    if (mensagem != NULL && strcmp(mensagem, "1") != 0)
    {
        linhaCol(1, 1);
        printf("\n%s\n", mensagem);
        printf("\nPressione ENTER para continuar...\n");
    }
}


/**
 * @brief Mostra texto com animação de digitação
 * @details Exibe palavra por palavra com delay
 */
void mostrarTextoAnimado(const char *texto, int linha, int coluna)
{
    char palavra[100];
    int i = 0, j = 0;
    linhaCol(1, 1);
    printf("                                                                                                                                ");
    linhaCol(2, 1);
    printf("                                                                                                                                ");
    linhaCol(3, 1);
    printf("                                                                                                                       ");
    linhaCol(1, coluna);

    while (texto[i] != '\0')
    {
        j = 0;
        while (texto[i] != ' ' && texto[i] != '\0')
        {
            palavra[j++] = texto[i++];
        }
        palavra[j] = '\0';


        printf("%s", palavra);
        fflush(stdout);
        Sleep(150);

        if (texto[i] == ' ')
        {
            printf(" ");
            i++;
        }
    }


    while(_kbhit()) getch();

    linhaCol(3, 1);
    printf("Continuar...");
    getch();
    linhaCol(3, 1);
    printf("                                                                                ");
}

/**
 * @brief Lê uma linha aleatória de um arquivo de texto
 * @details Conta linhas, escolhe uma aleatoriamente e a retorna
 */
char *lerFraseAleatoria(const char *arquivo)
{
    FILE *fp = fopen(arquivo, "r");
    if (!fp)
        return NULL;
    int num_linhas = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), fp))  num_linhas++;
    int linha_aleatoria = rand() % num_linhas;
    rewind(fp);
    static char frase_escolhida[256];
    for (int i = 0; i <= linha_aleatoria; i++)
    {
        if (fgets(frase_escolhida, sizeof(frase_escolhida), fp) == NULL)
        {
            fclose(fp);
            return NULL;
        }
    }
    fclose(fp);
    frase_escolhida[strcspn(frase_escolhida, "\n")] = 0;
    return frase_escolhida;
}