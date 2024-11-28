#ifndef JOGAR_H
#define JOGAR_H

void iniciar_jogo();
void mostrar_animacao_mesa();
int mostrar_cartas_menu(int jogador_id, char lista_cartas[][40]);
void iniciar_jogo(SOCKET clienteSocket, int jogador_id);

#endif // JOGAR_H

