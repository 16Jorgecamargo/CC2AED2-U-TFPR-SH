#ifndef MENU_H
#define MENU_H

#include "player.h"

#define menuLi 20  /**< Número de linhas do menu */
#define menuCo 80  /**< Número de colunas do menu */

/**
 * @brief Inicializa o layout do menu principal
 * @param menuLayout Matriz que representa o layout do menu
 */
void inicializarTelaMenu(char menuLayout[menuLi][menuCo]);

/**
 * @brief Renderiza o menu na tela
 * @param menuLayout Matriz do layout do menu
 * @param opcaoSelecionada Opção atualmente selecionada
 */
void renderizarMenu(char menuLayout[menuLi][menuCo], int opcaoSelecionada);

/**
 * @brief Exibe e gerencia o menu principal
 * @return Opção selecionada pelo usuário
 */
int menuPrincipal(void);

/**
 * @brief Exibe e gerencia o menu de ações do jogo
 * @param p Ponteiro para o personagem
 * @return Ação selecionada pelo usuário
 */
int menuAcoes(Personagem *p);

/**
 * @brief Exibe e gerencia o menu do inventário
 * @param p Ponteiro para o personagem
 * @return 1 se um item foi usado, 0 caso contrário
 */
int menuInventario(Personagem *p);

#endif
