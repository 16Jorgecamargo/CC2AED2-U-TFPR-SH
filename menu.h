#ifndef MENU_H
#define MENU_H

#include "player.h"

#define menuLi 20
#define menuCo 80

void inicializarTelaMenu(char menuL[menuLi][menuCo]);
void renderizarMenu(char menuL[menuLi][menuCo], int opcaoSelecionada);
int menuPrincipal(void);
int menuAcoes(Personagem *p);
int menuInventario(Personagem *p);

#endif
