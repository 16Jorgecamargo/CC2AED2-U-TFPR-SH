#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "menu.h"
#include "util.h"

/**
 * @brief Prepara o layout inicial do menu
 * @details Cria bordas e título do menu
 */
void inicializarTelaMenu(char menuL[menuLi][menuCo]) {
    for (int i = 0; i < menuLi; i++) {
        for (int j = 0; j < menuCo; j++) {
            menuL[i][j] = ' ';
        }
    }

    for (int j = 0; j < menuCo; j++) {
        menuL[0][j] = '-';
        menuL[menuLi-1][j] = '-';
    }
    
    for (int i = 0; i < menuLi; i++) {
        menuL[i][0] = '|';
        menuL[i][menuCo-1] = '|';
    }
    
    char titulo[] = "RPG de Dungeon";
    int tituloPos = (menuCo - strlen(titulo)) / 2;
    for (int i = 0; i < strlen(titulo); i++) {
        menuL[2][tituloPos + i] = titulo[i];
    }
}

/**
 * @brief Desenha o menu na tela com a opção selecionada
 * @details Atualiza a tela com as opções e destaca a selecionada
 */
void renderizarMenu(char menuL[menuLi][menuCo], int opcaoSelecionada) {
    limparTela("1");

    char menuAtual[menuLi][menuCo];
    for (int i = 0; i < menuLi; i++) {
        for (int j = 0; j < menuCo; j++) {
            menuAtual[i][j] = menuL[i][j];
        }
    }
    
    char *opcoes[] = {"Novo Jogo", "Carregar Jogo", "Sair"};
    for (int i = 0; i < 3; i++) {
        int linha = 8 + (i * 2);
        int coluna = (menuCo - strlen(opcoes[i])) / 2;
        
        if (i + 1 == opcaoSelecionada) {
            menuAtual[linha][coluna-2] = '>';
            menuAtual[linha][coluna+strlen(opcoes[i])+1] = '<';
        }
        
        for (int j = 0; j < strlen(opcoes[i]); j++) {
            menuAtual[linha][coluna + j] = opcoes[i][j];
        }
    }
    
    for (int i = 0; i < menuLi; i++) {
        for (int j = 0; j < menuCo; j++) {
            printf("%c", menuAtual[i][j]);
        }
        printf("\n");
    }
}

/**
 * @brief Gerencia o menu principal do jogo
 * @details Processa entrada do usuário e retorna a opção selecionada
 * @return Opção selecionada (1-3)
 */
int menuPrincipal() {
    char menuL[menuLi][menuCo];
    inicializarTelaMenu(menuL);
    int opc = 1;
    int tecla;
    while (1) {
        renderizarMenu(menuL, opc);
        tecla = getch();
        if (tecla == 72 && opc > 1) opc--;
        else if (tecla == 80 && opc < 3) opc++;
        else if (tecla == 13 || tecla == 32) return opc;
        else if (tecla == 27) return 3;
    }
}

/**
 * @brief Gerencia o menu de ações durante o jogo
 * @details Mostra status do personagem e opções disponíveis
 * @return Ação selecionada pelo jogador
 */
int menuAcoes(Personagem *p)
{
    char opcoes[4][20] = {"Explorar Dungeon", "Inventario", "Sair da Dungeon", "Voltar ao Menu"};
    int opc = 1;
    int tecla;

    while (1)
    {
        limparTela("1");
        if (p->ultima_mensagem[0] != '\0')
        {
            printf("\n%s\n", p->ultima_mensagem);
            printf("\n------------------------------------------------------------------------------------\n\n");
        }

        mostrarStatus(p);
        linhaCol(5, 45);
        printf("\t === Acoes ===");
        box(6, 45, 14, 68);

        for (int i = 0; i < 4; i++)
        {
            linhaCol(7 + i * 2, 47);
            if (i + 1 == opc) printf("> %s <", opcoes[i]);
            else printf("  %s  ", opcoes[i]);
        }
        tecla = getch();
        if (tecla == 72 && opc > 1) opc--;
        else if (tecla == 80 && opc < 4) opc++;
        else if (tecla == 13 || tecla == 32) return opc;
        else if (tecla == 27) return 4;
    }
}

/**
 * @brief Gerencia o menu do inventário
 * @details Permite visualizar e usar itens do inventário
 * @return 1 se um item foi usado, 0 se cancelou
 */
int menuInventario(Personagem *p)
{
    int opc = 0;
    int tecla;

    while (1)
    {
        limparTela("1");
        mostrarStatus(p);

        linhaCol(11, 1);
        printf("Inventario (%d/10):\n", p->num_itens);

        for (int i = 0; i < p->num_itens; i++)
        {
            linhaCol(12 + i, 1);
            if (i == opc)
            {
                printf("- %s <-", p->inventario[i]);
            }
            else
            {
                printf("- %s  ", p->inventario[i]);
            }
        }

        tecla = getch();
        if (tecla == 72 && opc > 0) opc--;
        else if (tecla == 80 && opc < p->num_itens - 1) opc++;
        else if (tecla == 13 || tecla == 32)
        {
            if (p->num_itens > 0)
            {
                linhaCol(25, 45);
                usarItem(p, opc);
                return 1;
            }
        }
        else if (tecla == 27) return 0;
        
    }
}