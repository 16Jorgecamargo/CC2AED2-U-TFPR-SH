# 🎮 RPG de Dungeon - Documentação Técnica

## 📋 Sumário
1. [Estrutura do Projeto](#estrutura-do-projeto)
2. [Aspectos Técnicos](#aspectos-técnicos)
3. [Sistemas Core](#sistemas-core)
4. [Mecânicas de Jogo](#mecânicas-de-jogo)
5. [Guia de Desenvolvimento](#guia-de-desenvolvimento)

## 🗂️ Estrutura do Projeto

### Organização dos Arquivos
```
RPG-Dungeon/
├── 📄 main.c              # Loop principal e gerenciamento de eventos
├── 📄 player.h/c          # Sistema de personagem
├── 📄 monstro.h/c         # Sistema de combate
├── 📄 menu.h/c            # Interface do usuário
├── 📄 util.h/c            # Funções utilitárias
└── 📁 eventos/            # Arquivos de eventos
    ├── entrada.txt        # Mensagens de entrada
    ├── saida.txt         # Mensagens de saída
    ├── monstro.txt       # Encontros com monstros
    ├── item.txt          # Descoberta de itens
    ├── evento.txt        # Eventos especiais
    └── caminho.txt       # Exploração normal
```

## 🔧 Aspectos Técnicos

### Dependências do Sistema
- **Sistema Operacional**: Windows
- **Compilador**: GCC (MinGW recomendado)
- **Bibliotecas Padrão**:
  - `<stdio.h>`: E/S padrão
  - `<stdlib.h>`: Alocação dinâmica
  - `<string.h>`: Manipulação de strings
  - `<time.h>`: Geração de números aleatórios
- **Bibliotecas Windows**:
  - `<windows.h>`: Interface do console
  - `<conio.h>`: Input do teclado

### Compilação e Execução
```bash
# Compilação
gcc main.c player.c monstro.c menu.c util.c -o rpg.exe -Wall

# Execução
./rpg.exe
```

## 💻 Sistemas Core

### Sistema de Personagem
```c
typedef struct {
    char nome[50];           // Nome do personagem
    int vida, vida_max;      // HP atual e máximo
    int ataque, defesa;      // Stats de combate
    int nivel, experiencia;  // Progressão
    char inventario[10][50]; // Array de itens
    int num_itens;          // Contador de itens
    char ultima_mensagem[256]; // Feedback ao jogador
} Personagem;
```

#### Funções Principais
- `criarPersonagem()`: Inicialização
- `mostrarStatus()`: UI de status
- `subirNivel()`: Progressão
- `usarItem()`: Consumíveis
- `salvarProgresso()`: Persistência
- `carregarProgresso()`: Load

### Sistema de Combate
```c
typedef struct {
    const char *nome;     // Identificador
    int vida_base;        // HP base
    int ataque_base;      // ATK base
    int defesa_base;      // DEF base
    int exp_base;         // EXP reward
} MonstroBase;
```

#### Balanceamento de Monstros
| Monstro | Vida | ATK | DEF | EXP |
|---------|------|-----|-----|-----|
| Aranha  | 40   | 5   | 2   | 45  |
| Golem   | 80   | 4   | 8   | 60  |
| Dragão  | 100  | 10  | 9   | 100 |

## 🎲 Mecânicas de Jogo

### Sistema de Eventos
- **Probabilidades**:
  - 15%: Encontro com monstro
  - 5%: Encontro de item
  - 2%: Evento especial
  - 78%: Exploração normal

### Fórmulas de Combate
```c
// Cálculo de Dano
dano_final = max(1, ataque_atacante - defesa_defensor);

// Experiência para Level Up
exp_necessaria = nivel_atual * 100;
```

### Sistema de Itens
- **Poção de Cura**: +50 HP
- **Poção de Força**: +5 ATK
- **Poção de Resistência**: +3 DEF
- **Poção de Sorte**: Efeitos especiais

## 📚 Guia de Desenvolvimento

### Convenções de Código
- Comentários em português
- Documentação em estilo Doxygen
- Indentação com 4 espaços
- Nomes de funções em camelCase

### Controle de Arquivos
```c
// Formato do Save
nome_personagem
vida vida_max ataque defesa nivel exp
num_itens
item1
item2
...
```

### Interface do Usuário
- **Controles**:
  - `↑/↓`: Navegação
  - `Enter/Space`: Confirmar
  - `ESC`: Voltar/Cancelar
- **Display**:
  - ASCII art para UI
  - Animação de texto
  - Boxes para menus

### Extensibilidade
- Adicionar monstros: Expandir array `MONSTROS`
- Novos itens: Modificar `usarItem()`
- Eventos: Adicionar arquivos .txt

## 🔍 Notas de Implementação

### Tratamento de Erros
- Verificação de alocação de memória
- Validação de arquivos
- Limites de inventário
- Valores mínimos de stats

### Performance
- Uso de buffer estático para strings
- Reutilização de memória alocada
- Otimização de loops de renderização

### Segurança
- Validação de input
- Limites em buffers
- Verificação de ponteiros NULL
