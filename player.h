#ifndef PLAYER_H
#define PLAYER_H

/**
 * @brief Estrutura que representa um personagem do jogo
 */
typedef struct {
    char nome[50];          /**< Nome do personagem */
    int vida;              /**< Vida atual do personagem */
    int vida_max;         /**< Vida máxima do personagem */
    int ataque;           /**< Poder de ataque do personagem */
    int defesa;           /**< Poder de defesa do personagem */
    int nivel;            /**< Nível atual do personagem */
    int experiencia;      /**< Pontos de experiência atuais */
    char inventario[10][50]; /**< Inventário com até 10 itens */
    int num_itens;        /**< Número atual de itens no inventário */
    char ultima_mensagem[256]; /**< Última mensagem do sistema */
} Personagem;

/**
 * @brief Cria um novo personagem com valores iniciais
 * @param p Ponteiro para a estrutura do personagem
 */
void criarPersonagem(Personagem *p);

/**
 * @brief Exibe o status atual do personagem
 * @param p Ponteiro para a estrutura do personagem
 */
void mostrarStatus(Personagem *p);

/**
 * @brief Aumenta o nível do personagem e seus atributos
 * @param p Ponteiro para a estrutura do personagem
 */
void subirNivel(Personagem *p);

/**
 * @brief Adiciona um item ao inventário do personagem
 * @param p Ponteiro para a estrutura do personagem
 * @param item Nome do item a ser adicionado
 */
void adicionarItem(Personagem *p, const char *item);

/**
 * @brief Usa um item do inventário
 * @param p Ponteiro para a estrutura do personagem
 * @param index Índice do item no inventário
 */
void usarItem(Personagem *p, int index);

/**
 * @brief Salva o progresso do personagem em arquivo
 * @param p Ponteiro para a estrutura do personagem
 * @param arquivo Nome do arquivo para salvar
 */
void salvarProgresso(Personagem *p, const char *arquivo);

/**
 * @brief Carrega o progresso do personagem de um arquivo
 * @param p Ponteiro para a estrutura do personagem
 * @param arquivo Nome do arquivo para carregar
 * @return 1 se sucesso, 0 se falha
 */
int carregarProgresso(Personagem *p, const char *arquivo);

#endif
