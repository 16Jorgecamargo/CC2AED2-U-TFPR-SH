#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "util.h"

/**
 * @brief Inicializa um novo personagem com valores base
 * @details Define valores iniciais para todos os atributos
 */
void criarPersonagem(Personagem *p)
{
    printf("Digite o nome do seu personagem: ");
    scanf("%s", p->nome);
    p->vida = p->vida_max = 100;
    p->ataque = 10;
    p->defesa = 5;
    p->nivel = 1;
    p->experiencia = 0;
    p->num_itens = 0;
    p->ultima_mensagem[0] = '\0';
}

/**
 * @brief Exibe informações do personagem na tela
 * @details Mostra status, atributos e inventário atual
 */
void mostrarStatus(Personagem *p)
{
    linhaCol(5, 1);
    printf("=== Status de %s ===\n", p->nome);
    printf("Nivel: %d\n", p->nivel);
    printf("Vida: %d/%d\n", p->vida, p->vida_max);
    printf("Ataque: %d\n", p->ataque);
    printf("Defesa: %d\n", p->defesa);
    printf("Experiencia: %d\n", p->experiencia);
    printf("Inventario (%d/10):\n", p->num_itens);
    for (int i = 0; i < p->num_itens; i++)
    {
        printf("- %s\n", p->inventario[i]);
    }
}

/**
 * @brief Processa a evolução de nível do personagem
 * @details Aumenta atributos base e restaura vida
 */
void subirNivel(Personagem *p)
{
    p->nivel++;
    p->vida_max += 20;
    p->vida = p->vida_max;
    p->ataque += 5;
    p->defesa += 3;
    printf("\nParabens! Voce subiu para o nivel %d!\n", p->nivel);
}

/**
 * @brief Processa o uso de itens do inventário
 * @details Implementa efeitos diferentes para cada tipo de poção
 */
void usarItem(Personagem *p, int index)
{
    if (index < 0 || index >= p->num_itens)
        return;

    if (strcmp(p->inventario[index], "Pocao de Cura") == 0)
    {
        int cura = 50;
        p->vida = minimo(p->vida + cura, p->vida_max);
        printf("\nVoce recuperou %d de vida!", cura);
    }
    else if (strcmp(p->inventario[index], "Pocao de Forca") == 0)
    {
        int bonus = 5;
        p->ataque += bonus;
        printf("\nSeu ataque aumentou em %d!", bonus);
    }
    else if (strcmp(p->inventario[index], "Pocao de Sorte") == 0)
    {
        printf("\nVoce se sente mais sortudo!");
    }
    else if (strcmp(p->inventario[index], "Pocao de Resistencia") == 0)
    {
        int bonus = 3;
        p->defesa += bonus;
        printf("\nSua defesa aumentou em %d!", bonus);
    }

    for (int i = index; i < p->num_itens - 1; i++)
    {
        strcpy(p->inventario[i], p->inventario[i + 1]);
    }
    p->num_itens--;
}

/**
 * @brief Gerencia adição de itens ao inventário
 * @details Verifica limite de itens antes de adicionar
 */
void adicionarItem(Personagem *p, const char *item)
{
    if (p->num_itens < 10)
    {
        strcpy(p->inventario[p->num_itens], item);
        p->num_itens++;
        printf("Item adicionado: %s\n", item);
    }
    else
    {
        printf("Inventario cheio!\n");
    }
}

/**
 * @brief Salva o estado atual do personagem em arquivo
 * @details Grava todos os atributos e itens do inventário
 */
void salvarProgresso(Personagem *p, const char *arquivo)
{
    FILE *fp = fopen(arquivo, "w");
    if (!fp)
    {
        printf("Erro ao abrir arquivo para salvar.\n");
        return;
    }

    fprintf(fp, "%s\n", p->nome);
    fprintf(fp, "%d %d %d %d %d %d\n",
            p->vida, p->vida_max, p->ataque, p->defesa, p->nivel, p->experiencia);
    fprintf(fp, "%d\n", p->num_itens);

    for (int i = 0; i < p->num_itens; i++)
    {
        fprintf(fp, "%s\n", p->inventario[i]);
    }

    fclose(fp);
}

/**
 * @brief Carrega o estado do personagem de um arquivo
 * @details Lê e valida todos os dados salvos
 */
int carregarProgresso(Personagem *p, const char *arquivo)
{
    FILE *fp = fopen(arquivo, "r");
    if (!fp)
    {
        return 0;
    }

    char buffer[50];
    if (fgets(buffer, sizeof(buffer), fp) == NULL)
    {
        fclose(fp);
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(p->nome, buffer, sizeof(p->nome) - 1);

    if (fscanf(fp, "%d %d %d %d %d %d",
               &p->vida, &p->vida_max, &p->ataque, &p->defesa, &p->nivel, &p->experiencia) != 6)
    {
        fclose(fp);
        return 0;
    }

    if (fscanf(fp, "%d", &p->num_itens) != 1)
    {
        fclose(fp);
        return 0;
    }

    fgetc(fp);

    for (int i = 0; i < p->num_itens && i < 10; i++)
    {
        if (fgets(buffer, sizeof(buffer), fp) == NULL)
        {
            fclose(fp);
            return 0;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(p->inventario[i], buffer, sizeof(p->inventario[i]) - 1);
    }

    fclose(fp);
    return 1;
}