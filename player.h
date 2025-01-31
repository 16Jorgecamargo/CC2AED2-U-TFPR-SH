#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    char nome[50];
    int vida;
    int vida_max;
    int ataque;
    int defesa;
    int nivel;
    int experiencia;
    char inventario[10][50];
    int num_itens;
    char ultima_mensagem[256];
} Personagem;

void criarPersonagem(Personagem *p);
void mostrarStatus(Personagem *p);
void subirNivel(Personagem *p);
void adicionarItem(Personagem *p, const char *item);
void usarItem(Personagem *p, int index);
void salvarProgresso(Personagem *p, const char *arquivo);
int carregarProgresso(Personagem *p, const char *arquivo);

#endif
