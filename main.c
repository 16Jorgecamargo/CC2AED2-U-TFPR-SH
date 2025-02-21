#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "player.h"
#include "monstro.h"
#include "menu.h"
#include "util.h"

/**
 * @brief Função principal do jogo
 * @details Gerencia o fluxo principal do jogo, incluindo:
 * - Inicialização do jogo
 * - Menu principal
 * - Loop principal de jogabilidade
 * - Sistema de eventos aleatórios
 * - Gerenciamento de save/load
 * 
 * @return 0 em caso de término normal
 */
int main()
{
    srand(time(NULL));
    Personagem jogador;
    int escolha;

    while (1)
    {
        escolha = menuPrincipal();

        // Processamento do menu principal
        if (escolha == 3)
        {
            printf("\nObrigado por jogar!\n");
            return 0;
        }

        if (escolha == 2)
        {
            if (!carregarProgresso(&jogador, "save.txt"))
            {
                printf("Erro ao carregar jogo. Iniciando novo jogo...\n");
                printf("Pressione ENTER para continuar...");
                getchar();
                getchar();
                criarPersonagem(&jogador);
            }
            else
            {
                char *entrada = lerFraseAleatoria("eventos/entrada.txt");
                if (entrada)
                {
                    limparTela("1");
                    strcpy(jogador.ultima_mensagem, entrada);
                    mostrarTextoAnimado(entrada, 1, 1);
                }
            }
        }

        else if (escolha == 1)
        {
            limparTela("1");
            criarPersonagem(&jogador);
        }

        // Loop principal do jogo
        while (1)
        {
            limparTela("1");
            mostrarStatus(&jogador);

            escolha = menuAcoes(&jogador);

            // Sistema de eventos aleatórios
            switch (escolha)
            {
            case 1:
            {
                int chance = rand() % 100;

                if (chance < 20)
                { // 20% chance de monstro
                    char *mensagem = lerFraseAleatoria("eventos/monstro.txt");
                    if (mensagem)
                    {
                        // Gera o inimigo primeiro para ter o nome
                        MonstroBase *monstro = gerarMonstro(jogador.nivel);
                        if (monstro)
                        {
                            // Cria um buffer para a mensagem formatada
                            char mensagem_formatada[256];
                            sprintf(mensagem_formatada, mensagem, monstro->nome);

                            // Mostra a mensagem com o nome do monstro
                            strcpy(jogador.ultima_mensagem, mensagem_formatada);
                            mostrarTextoAnimado(mensagem_formatada, 1, 1);
                            limparTela("1");

                            // Inicia batalha
                            batalha(&jogador, monstro);
                            free(monstro);
                        }
                    }
                }
                else if (chance < 25)
                { // 25% chance de item
                    char *item = lerFraseAleatoria("eventos/item.txt");
                    if (item)
                    {
                        strcpy(jogador.ultima_mensagem, item);
                        mostrarTextoAnimado(item, 1, 1);
                        // Adiciona uma poção aleatória
                        const char *pocoes[] = {"Pocao de Cura", "Pocao de Forca", "Pocao de Sorte", "Pocao de Resistencia"};
                        adicionarItem(&jogador, pocoes[rand() % 4]);
                    }
                }
                else if (chance < 22)
                { // 22% chance de evento
                    char *evento = lerFraseAleatoria("eventos/evento.txt");
                    if (evento)
                    {
                        strcpy(jogador.ultima_mensagem, evento);
                        mostrarTextoAnimado(evento, 1, 1);
                        // Processa o efeito do evento
                        if (strstr(evento, "[+"))
                        {
                            if (strstr(evento, "Vida]"))
                                jogador.vida = minimo(jogador.vida + 2, jogador.vida_max);
                            if (strstr(evento, "Ataque]"))
                                jogador.ataque += 1;
                            if (strstr(evento, "Defesa]"))
                                jogador.defesa += 1;
                            if (strstr(evento, "Experiencia]"))
                                jogador.experiencia += 2;
                            if (strstr(evento, "Item Aleatorio]"))
                            {
                                const char *pocoes[] = {"Pocao de Cura", "Pocao de Forca", "Pocao de Sorte", "Pocao de Resistencia"};
                                adicionarItem(&jogador, pocoes[rand() % 4]);
                            }
                        }
                        else if (strstr(evento, "[-"))
                        {
                            if (strstr(evento, "Vida]"))
                                jogador.vida = maximo(1, jogador.vida - 2);
                            if (strstr(evento, "Ataque]"))
                                jogador.ataque = maximo(1, jogador.ataque - 1);
                            if (strstr(evento, "Defesa]"))
                                jogador.defesa = maximo(0, jogador.defesa - 1);
                            if (strstr(evento, "Experiencia]"))
                                jogador.experiencia = maximo(0, jogador.experiencia - 2);
                            if (strstr(evento, "Item Aleatorio]") && jogador.num_itens > 0)
                            {
                                // Remove um item aleatório
                                int item_remover = rand() % jogador.num_itens;
                                for (int i = item_remover; i < jogador.num_itens - 1; i++)
                                {
                                    strcpy(jogador.inventario[i], jogador.inventario[i + 1]);
                                }
                                jogador.num_itens--;
                            }
                        }
                    }
                }
                else
                { // 33% chance de caminho
                    char *caminho = lerFraseAleatoria("eventos/caminho.txt");
                    if (caminho)
                    {
                        strcpy(jogador.ultima_mensagem, caminho);
                        mostrarTextoAnimado(caminho, 1, 1);
                    }
                }
                break;
            }
            case 2: // Inventário
                if (menuInventario(&jogador))
                {
                    printf("\nPressione ENTER para continuar...");
                    getchar();
                }
                break;
            case 3:
            {
                char *saida = lerFraseAleatoria("eventos/saida.txt");
                if (saida)
                {
                    strcpy(jogador.ultima_mensagem, saida);
                    salvarProgresso(&jogador, "save.txt");
                    mostrarTextoAnimado(saida, 1, 1);
                }
                goto menu_principal;
                break;
            }
            case 4:
                goto menu_principal;
            }

            if (jogador.vida <= 0)
            {
                limparTela("1");
                printf("\nGame Over! %s morreu na dungeon...\n", jogador.nome);
                printf("Pressione ENTER para continuar...");
                getchar();
                break;
            }
        }
    menu_principal:;
    }

    return 0;
}
