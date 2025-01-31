#ifndef MONSTRO_H
#define MONSTRO_H

#include "player.h"

typedef struct
{
    const char *nome;
    int vida_base;
    int ataque_base;
    int defesa_base;
    int exp_base;
} MonstroBase;

void batalha(Personagem *p, MonstroBase *monstro);
MonstroBase* gerarMonstro(int nivel_jogador);

#endif
