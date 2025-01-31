#ifndef MONSTRO_H
#define MONSTRO_H

#include "player.h"

/**
 * @brief Estrutura que define as características base de um monstro
 */
typedef struct {
    const char *nome;     /**< Nome do monstro */
    int vida_base;        /**< Vida base do monstro */
    int ataque_base;      /**< Ataque base do monstro */
    int defesa_base;      /**< Defesa base do monstro */
    int exp_base;         /**< Experiência fornecida ao ser derrotado */
} MonstroBase;

/**
 * @brief Inicia uma batalha entre o personagem e um monstro
 * @param p Ponteiro para o personagem
 * @param monstro Ponteiro para o monstro
 */
void batalha(Personagem *p, MonstroBase *monstro);

/**
 * @brief Gera um monstro com atributos baseados no nível do jogador
 * @param nivel_jogador Nível atual do jogador
 * @return Ponteiro para o monstro criado
 */
MonstroBase* gerarMonstro(int nivel_jogador);

#endif
