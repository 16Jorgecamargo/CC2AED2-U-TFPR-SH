# 🎮 RPG de Dungeon em C

Um jogo de RPG baseado em texto desenvolvido em C, com sistema de combate por turnos, inventário e exploração de dungeon.

## 🛠️ Tecnologias Utilizadas

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![VSCode](https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Git](https://img.shields.io/badge/GIT-E44C30?style=for-the-badge&logo=git&logoColor=white)

## ✅ Checklist de Conceitos Implementados
- [x] **💻 Funções de Entrada e Saída**
  - Uso de printf/scanf para interação com usuário
  - Funções personalizadas de entrada/saída (mostrarTextoAnimado, mostrarStatus)

- [x] **🔄 Estruturas de Controle**
  - Loops while para menus
  - Switch/case para escolhas do jogador
  - Condicionais if/else para lógica do jogo

- [x] **👉 Ponteiros**
  - Uso de ponteiros para estrutura Personagem
  - Ponteiros para MonstroBase
  - Passagem de ponteiros entre funções

- [x] **📊 Alocação Dinâmica**
  - Alocação dinâmica de monstros (gerarMonstro)
  - Desalocação apropriada após uso (free em batalha)

- [x] **📝 Funções com Vetores/Matrizes**
  - menuLayout usando matriz para interface
  - Passagem de arrays em funções de menu
  - Manipulação de inventário em funções

- [x] **📚 Estruturas Homogêneas**
  - Arrays de strings para inventário
  - Matriz para layout do menu
  - Vetores para opções de menu

- [x] **🏗️ Estruturas Heterogêneas**
  - Struct Personagem
  - Struct MonstroBase
  - Passagem de estruturas por referência

- [x] **📁 Arquivos**
  - Leitura de eventos aleatórios (lerFraseAleatoria)
  - Sistema de save/load (salvarProgresso/carregarProgresso)
  - Manipulação adequada de arquivos (fopen/fclose)

- [x] **📦 Divisão em Módulos**
  - player.h/c: Sistema de personagem
  - monstro.h/c: Sistema de combate
  - menu.h/c: Interface do usuário
  - util.h/c: Funções utilitárias
  - main.c: Arquivo principal

## 🎯 Características

### ⚔️ Sistema de Combate
- Batalhas por turno contra diferentes tipos de monstros
- 10 tipos diferentes de monstros com atributos únicos
- Sistema de dano baseado em ataque e defesa
- Opção de fugir do combate

### 🦸‍♂️ Personagem
- Sistema de níveis e experiência
- Atributos: vida, ataque e defesa
- Inventário com capacidade para 10 itens
- Sistema de uso de poções e itens

### 🧪 Itens
- Poção de Cura: Recupera 50 pontos de vida
- Poção de Força: Aumenta o ataque
- Poção de Resistência: Aumenta a defesa
- Poção de Sorte: Efeitos especiais

### 🎲 Sistema de Eventos
- Eventos aleatórios durante a exploração
- 15% chance de encontrar monstros
- 5% chance de encontrar itens
- 2% chance de eventos especiais
- 78% chance de exploração normal

### 👾 Monstros Disponíveis
1. Aranha Gigante
2. Golem de Pedra
3. Bruxa Sombria
4. Esqueleto Guerreiro
5. Slime Ácido
6. Minotauro
7. Vampiro Ancião
8. Quimera
9. Hidra Jovem
10. Dragão Filhote

### 💾 Sistema de Save
- Salvar progresso do jogador
- Carregar jogos salvos
- Mantem todos os atributos e inventário

### 🖥️ Interface
- Menu principal com opções de novo jogo e carregar jogo
- Sistema de menus navegáveis
- Animações de texto
- Interface em ASCII art

## 💻 Requisitos do Sistema
- Sistema Operacional Windows
- Compilador C
- Biblioteca windows.h para interface

## 📁 Estrutura do Projeto

### 📂 Arquivos do Sistema
```
📦 RPG-Dungeon
├── 📜 main.c
├── 📜 player.h/c
├── 📜 monstro.h/c
├── 📜 menu.h/c
├── 📜 util.h/c
└── 📂 eventos/
    ├── 📄 entrada.txt
    ├── 📄 saida.txt
    ├── 📄 monstro.txt
    ├── 📄 item.txt
    ├── 📄 evento.txt
    └── 📄 caminho.txt
```

### ⚠️ Validações e Tratamento de Erros
- Verificação de abertura de arquivos
- Validação de alocação de memória
- Tratamento de inventário cheio
- Verificação de itens disponíveis
- Validação de carregamento de jogo

## 📚 Documentação das Funcionalidades

### 🎮 Sistema Principal
- Gerenciamento de estados do jogo
- Sistema de eventos aleatórios
- Processamento de batalhas
- Gestão de inventário

### 👤 Sistema de Personagem
- Criação e desenvolvimento de personagem
- Sistema de experiência e níveis
- Gerenciamento de atributos
- Manipulação de inventário

### ⚔️ Sistema de Combate
- Batalhas por turno
- Cálculo de dano
- Balanceamento de monstros
- Sistema de fuga

### 📁 Sistema de Arquivos
- Salvamento de progresso
- Carregamento de jogos
- Leitura de eventos aleatórios
- Persistência de dados

## 🚀 Como Compilar
```bash
gcc main.c player.c monstro.c menu.c util.c -o rpg.exe
```

## 🎮 Como Jogar
1. 🏃‍♂️ Execute o jogo
2. 📝 Escolha entre novo jogo ou carregar jogo
3. 👤 Em novo jogo, crie seu personagem
4. ⌨️ Use as setas para navegar nos menus
5. ↩️ Enter ou Espaço para selecionar
6. ❌ ESC para voltar/cancelar

## 👥 Desenvolvedores
- 👨‍💻 Jorge Camargo
- 👨‍💻 João Pedro
- 👨‍💻 Orias Malta Dos Santos Junior
- 👨‍💻 RRian Augusto Amaral
