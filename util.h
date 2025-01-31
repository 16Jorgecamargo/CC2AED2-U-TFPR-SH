#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Posiciona o cursor na tela nas coordenadas especificadas
 * @param lin Linha onde o cursor será posicionado
 * @param col Coluna onde o cursor será posicionado
 */
void linhaCol(int lin, int col);

/**
 * @brief Desenha uma caixa na tela usando caracteres ASCII
 * @param lin1 Linha inicial da caixa
 * @param col1 Coluna inicial da caixa
 * @param lin2 Linha final da caixa
 * @param col2 Coluna final da caixa
 */
void box(int lin1, int col1, int lin2, int col2);

/**
 * @brief Exibe texto com animação palavra por palavra
 * @param texto Texto a ser exibido
 * @param linha Linha onde o texto será exibido
 * @param coluna Coluna onde o texto será exibido
 */
void mostrarTextoAnimado(const char* texto, int linha, int coluna);

/**
 * @brief Limpa a tela e exibe uma mensagem opcional
 * @param mensagem Mensagem a ser exibida após limpar a tela (NULL ou "1" para não exibir)
 */
void limparTela(const char* mensagem);

/**
 * @brief Retorna o menor valor entre dois números
 * @param a Primeiro número
 * @param b Segundo número
 * @return O menor valor entre a e b
 */
int minimo(int a, int b);

/**
 * @brief Retorna o maior valor entre dois números
 * @param a Primeiro número
 * @param b Segundo número
 * @return O maior valor entre a e b
 */
int maximo(int a, int b);

/**
 * @brief Lê uma frase aleatória de um arquivo
 * @param arquivo Caminho do arquivo contendo as frases
 * @return Ponteiro para a frase lida ou NULL em caso de erro
 */
char* lerFraseAleatoria(const char* arquivo);

#endif
