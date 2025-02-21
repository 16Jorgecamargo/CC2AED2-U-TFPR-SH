#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "monstro.h"
#include "menu.h"
#include "util.h"

/**
 * @brief Array com os tipos de monstros base do jogo
 * @details Cada monstro possui atributos base diferentes
 */
const MonstroBase MONSTROS[] = {
    {"Aranha Gigante", 40, 5, 2, 45},      
    {"Golem de Pedra", 80, 4, 8, 60},      
    {"Bruxa Sombria", 45, 8, 1, 55},      
    {"Esqueleto Guerreiro", 50, 6, 4, 50}, 
    {"Slime Acido", 35, 7, 0, 45},         
    {"Minotauro", 70, 9, 5, 65},           
    {"Vampiro Anciao", 55, 7, 3, 70},      
    {"Quimera", 65, 8, 6, 75},            
    {"Hidra Jovem", 90, 6, 7, 80},         
    {"Dragao Filhote", 100, 10, 9, 100}    
};

/**
 * @brief Sistema de batalha por turnos
 * @details Gerencia as ações do jogador e do monstro, calculando dano e experiência
 * @param p Ponteiro para o personagem do jogador
 * @param monstro Ponteiro para o monstro enfrentado
 */

void LimparBuffer(){
    while (kbhit()) {
        getch();
    }
}


void batalha(Personagem *p, MonstroBase *monstro) {
    int opc = 1;
    int tecla;
    int vida_monstro = monstro->vida_base;
    
    while (p->vida > 0 && vida_monstro > 0) {
        limparTela("1");
        printf("\n=== BATALHA ===\n");
        printf("\n%s    vs    %s\nHP: %d/%d - HP: %d\n",
               p->nome, monstro->nome, p->vida, p->vida_max, vida_monstro);

        box(8, 2, 14, 20);
        
        const char *opcoes[] = {"Atacar", "Usar Item", "Fugir"};
        for (int i = 0; i < 3; i++) {
            linhaCol(9 + i * 2, 4);
            if (i + 1 == opc) {
                printf("> %s <", opcoes[i]);
            } else {
                printf("  %s  ", opcoes[i]);
            }
        }

        linhaCol(18, 2);

        tecla = getch();
        
        if(tecla==224){
            tecla = getch();
        }
        if (tecla == 72 && opc > 1) { opc--; }
        else if (tecla == 80 && opc < 3) { opc++; }
        else if (tecla == 13 || tecla == 32) {
            LimparBuffer();
            if (opc == 3) {
                strcpy(p->ultima_mensagem, "Voce fugiu do monstro, nem item nem experiencia obtido.");
                Sleep(600);
                return;
            }
            else if (opc == 2) {
                if (p->num_itens > 0) {
                    if (menuInventario(p)) {
                         Sleep(600); 
                         LimparBuffer();
                        continue; }
                } else {
                    linhaCol(18, 2);
                    printf("Voce nao tem itens!");
                    Sleep(600);
                    LimparBuffer();
                    continue;
                }
            }
            else if (opc == 1) {
                int dano = p->ataque - monstro->defesa_base;
                if (dano < 1) dano = 1;
                vida_monstro -= dano;
                linhaCol(18, 2);
                printf("%s causou %d de dano!", p->nome, dano);
                Sleep(700);
                LimparBuffer();

                if (vida_monstro <= 0) {
                    char mensagem_vitoria[256];
                    sprintf(mensagem_vitoria, "Voce se sente aliviado por ter derrotado %s recebendo (%d) pontos de experiencia", 
                            monstro->nome, monstro->exp_base);
                    
                    strcpy(p->ultima_mensagem, mensagem_vitoria);
                    linhaCol(19, 2);
                    printf("Voce derrotou %s!", monstro->nome);
                    p->experiencia += monstro->exp_base;
                    
                    if (p->experiencia >= p->nivel * 100) {
                        subirNivel(p);
                        p->experiencia = 0;
                    }
                    Sleep(600);
                    LimparBuffer();
                    return;
                }

                dano = monstro->ataque_base - p->defesa;
                if (dano < 1) dano = 1;
                p->vida -= dano;
                linhaCol(19, 2);
                printf("%s causou %d de dano!", monstro->nome, dano);
                Sleep(600);
                LimparBuffer();
            }
        }
    }
}

/**
 * @brief Cria um novo monstro com base no nível do jogador
 * @details Ajusta os atributos do monstro de acordo com o nível do jogador
 * @param nivel_jogador Nível atual do jogador
 * @return Ponteiro para o novo monstro criado
 */
MonstroBase* gerarMonstro(int nivel_jogador) {
    MonstroBase* m = malloc(sizeof(MonstroBase));
    int tipo = rand() % 10;
    
    m->nome = MONSTROS[tipo].nome;
    m->vida_base = MONSTROS[tipo].vida_base + (nivel_jogador * 8);
    m->ataque_base = MONSTROS[tipo].ataque_base + (nivel_jogador * 2);
    m->defesa_base = MONSTROS[tipo].defesa_base + (nivel_jogador);
    m->exp_base = MONSTROS[tipo].exp_base + (nivel_jogador * 10);
    
    return m;
}