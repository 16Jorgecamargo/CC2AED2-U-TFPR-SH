#ifndef UTIL_H
#define UTIL_H

void linhaCol(int lin, int col);
void box(int lin1, int col1, int lin2, int col2);
void mostrarTextoAnimado(const char* texto, int linha, int coluna);
void limparTela(const char* mensagem);
int minimo(int a, int b);
int maximo(int a, int b);
char* lerFraseAleatoria(const char* arquivo);

#endif
