#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 9090
#define MAX_JOGADORES 4
#define BUFFER_TAM 1024

typedef struct
{
    SOCKET socket;
    int id;
    int ativo;
    int pronto_para_jogar;
} Jogador;

Jogador jogadores[MAX_JOGADORES] = {0}; // Lista de jogadores conectados
SOCKET servidorSocket;

const char *baralho[] =
{
    "Rei", "Rei", "Rei", "Rei", "Rei", "Rei",
    "Rainha", "Rainha", "Rainha", "Rainha", "Rainha", "Rainha",
    "Ás", "Ás", "Ás", "Ás", "Ás", "Ás",
    "Coringa", "Coringa"
};

typedef struct
{
    char cartas[5][40];
} MaoJogador;

MaoJogador maos_jogadores[MAX_JOGADORES];
char mesa_escolhida[40];
int cartas_usadas[20] = {0}; // Para controlar quais cartas já foram distribuídas

typedef struct
{
    int jogador_id;
    char carta[40];
    int jogada_realizada;
} JogadaRodada;

JogadaRodada jogadas_atuais[MAX_JOGADORES] = {0};
int jogadores_jogaram = 0;

void processar_jogada(int jogador_id, const char* carta)
{
    // Registra a jogada
    jogadas_atuais[jogador_id].jogador_id = jogador_id;
    strcpy(jogadas_atuais[jogador_id].carta, carta);
    jogadas_atuais[jogador_id].jogada_realizada = 1;
    jogadores_jogaram++;

    printf("Jogador %d escolheu: %s\n", jogador_id + 1, carta);

    // Verifica se todos os jogadores ativos já jogaram
    int jogadores_ativos = 0;
    for (int i = 0; i < MAX_JOGADORES; i++)
    {
        if (jogadores[i].ativo)
        {
            jogadores_ativos++;
        }
    }

    if (jogadores_jogaram == jogadores_ativos)
    {
        // Todos jogaram - processa a rodada
        char mensagem_resultado[BUFFER_TAM];
        sprintf(mensagem_resultado, "RESULTADO_RODADA:");

        // Adiciona as jogadas de cada jogador à mensagem
        for (int i = 0; i < MAX_JOGADORES; i++)
        {
            if (jogadores[i].ativo)
            {
                char jogada[100];
                sprintf(jogada, "Jogador %d: %s,", i + 1, jogadas_atuais[i].carta);
                strcat(mensagem_resultado, jogada);
            }
        }

        // Envia o resultado para todos os jogadores
        for (int i = 0; i < MAX_JOGADORES; i++)
        {
            if (jogadores[i].ativo)
            {
                enviar_mensagem_para_jogador(i, mensagem_resultado);
            }
        }

        // Reseta o estado para a próxima rodada
        memset(jogadas_atuais, 0, sizeof(jogadas_atuais));
        jogadores_jogaram = 0;
    }
}

void distribuir_cartas_jogador(int jogador_id)
{
    char mensagem[BUFFER_TAM];
    sprintf(mensagem, "CARTAS:");

    // Distribui 5 cartas para o jogador
    for (int i = 0; i < 5; i++)
    {
        int carta_aleatoria;
        do
        {
            carta_aleatoria = rand() % (sizeof(baralho) / sizeof(baralho[0]));
        }
        while (cartas_usadas[carta_aleatoria]);

        cartas_usadas[carta_aleatoria] = 1;
        strcpy(maos_jogadores[jogador_id].cartas[i], baralho[carta_aleatoria]);

        // Adiciona a carta à mensagem
        strcat(mensagem, maos_jogadores[jogador_id].cartas[i]);
        if (i < 4) strcat(mensagem, ",");
    }

    printf("Enviando cartas para jogador %d: %s\n", jogador_id + 1, mensagem);  // Debug
    enviar_mensagem_para_jogador(jogador_id, mensagem);
}

void escolher_mesa()
{
    const char *mesas[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de Aces"};
    int mesa_escolhida_index = rand() % 3;
    strcpy(mesa_escolhida, mesas[mesa_escolhida_index]);

    // Enviar a mesa escolhida para todos os jogadores
    char mensagem[BUFFER_TAM];
    sprintf(mensagem, "MESA_ESCOLHIDA:%s", mesa_escolhida);

    for (int i = 0; i < MAX_JOGADORES; i++)
    {
        if (jogadores[i].ativo)
        {
            enviar_mensagem_para_jogador(i, mensagem);
        }
    }
}

void enviar_mensagem_para_jogador(int jogadorID, const char *mensagem)
{
    if (jogadores[jogadorID].ativo)
    {
        if (send(jogadores[jogadorID].socket, mensagem, strlen(mensagem), 0) == SOCKET_ERROR)
        {
            printf("Erro ao enviar mensagem para Jogador %d. Codigo: %d\n", jogadores[jogadorID].id, WSAGetLastError());
        }
    }
}

void conectar_jogador(SOCKET novoSocket, int jogadorID)
{
    char buffer[BUFFER_TAM];

    jogadores[jogadorID].socket = novoSocket;
    jogadores[jogadorID].id = jogadorID + 1;
    jogadores[jogadorID].ativo = 1;
    jogadores[jogadorID].pronto_para_jogar = 0;

    printf("Jogador %d conectado.\n", jogadores[jogadorID].id);

    // Enviar mensagem de boas-vindas ao novo jogador
    sprintf(buffer, "Bem-vindo, Jogador %d!\n", jogadores[jogadorID].id);
    if (send(jogadores[jogadorID].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        printf("Erro ao enviar mensagem de boas-vindas para Jogador %d. Codigo: %d\n", jogadores[jogadorID].id, WSAGetLastError());
    }
}

void desconectar_jogador(int jogadorID)
{
    char buffer[BUFFER_TAM];

    printf("Jogador %d desconectou.\n", jogadores[jogadorID].id);

    // Avisar os outros jogadores sobre a desconexão
    sprintf(buffer, "Jogador %d desconectou.\n", jogadores[jogadorID].id);
    for (int i = 0; i < MAX_JOGADORES; i++)
    {
        if (jogadores[i].ativo && i != jogadorID)
        {
            if (send(jogadores[i].socket, buffer, strlen(buffer), 0) == SOCKET_ERROR)
            {
                printf("Erro ao enviar notificacao de desconexao para Jogador %d. Codigo: %d\n", jogadores[i].id, WSAGetLastError());
            }
        }
    }

    // Fechar o socket do jogador e marcar como inativo
    closesocket(jogadores[jogadorID].socket);
    jogadores[jogadorID].ativo = 0;
    jogadores[jogadorID].pronto_para_jogar = 0;
}

void verificar_jogadores_prontos()
{
    int prontos = 0;
    int jogadoresAtivos = 0;

    // Conta quantos jogadores estão prontos e ativos
    for (int i = 0; i < MAX_JOGADORES; i++)
    {
        if (jogadores[i].ativo)
        {
            jogadoresAtivos++;
            if (jogadores[i].pronto_para_jogar)
            {
                prontos++;
            }
        }
    }

    printf("Jogadores prontos: %d/%d\n", prontos, jogadoresAtivos);

    // Se todos os jogadores ativos estão prontos
    if (prontos == jogadoresAtivos && jogadoresAtivos >= 2)
    {
        printf("Todos os jogadores estão prontos. Enviando mensagens apropriadas...\n");

        // Envia mensagem específica para cada jogador
        for (int i = 0; i < MAX_JOGADORES; i++)
        {
            if (jogadores[i].ativo)
            {
                if (i == 0)   // Jogador 1
                {
                    printf("Enviando menu de início para jogador 1\n");
                    enviar_mensagem_para_jogador(i, "MOSTRAR_MENU_INICIAR");
                }
                else     // Outros jogadores
                {
                    printf("Enviando mensagem de aguardo para jogador %d\n", i + 1);
                    enviar_mensagem_para_jogador(i, "AGUARDAR_JOGADOR1");
                }
            }
        }
    }
}

void loop_principal()
{
    struct sockaddr_in endereco;
    int tamEndereco = sizeof(endereco);
    fd_set leituraSockets;
    int maxSocket;

    while (1)
    {
        FD_ZERO(&leituraSockets);
        FD_SET(servidorSocket, &leituraSockets);
        maxSocket = servidorSocket;

        for (int i = 0; i < MAX_JOGADORES; i++)
        {
            if (jogadores[i].ativo)
            {
                FD_SET(jogadores[i].socket, &leituraSockets);
                if (jogadores[i].socket > maxSocket)
                {
                    maxSocket = jogadores[i].socket;
                }
            }
        }

        int selectResult = select(maxSocket + 1, &leituraSockets, NULL, NULL, NULL);
        if (selectResult == SOCKET_ERROR)
        {
            printf("Erro no select. Codigo: %d\n", WSAGetLastError());
            break;
        }

        // Nova conexão
        if (FD_ISSET(servidorSocket, &leituraSockets))
        {
            SOCKET novoSocket = accept(servidorSocket, (struct sockaddr *)&endereco, &tamEndereco);

            int conexoesAtivas = 0;
            for (int i = 0; i < MAX_JOGADORES; i++)
            {
                if (jogadores[i].ativo)
                {
                    conexoesAtivas++;
                }
            }

            if (conexoesAtivas >= MAX_JOGADORES)
            {
                char *mensagem = "Servidor cheio. Tente novamente mais tarde.\n";
                send(novoSocket, mensagem, strlen(mensagem), 0);
                closesocket(novoSocket);
                printf("Conexao recusada. Numero maximo de jogadores atingido.\n");
            }
            else
            {
                for (int i = 0; i < MAX_JOGADORES; i++)
                {
                    if (!jogadores[i].ativo)
                    {
                        conectar_jogador(novoSocket, i);
                        break;
                    }
                }
            }
        }

        // Processar mensagens dos jogadores conectados
        for (int i = 0; i < MAX_JOGADORES; i++)
        {
            if (jogadores[i].ativo && FD_ISSET(jogadores[i].socket, &leituraSockets))
            {
                char buffer[BUFFER_TAM] = {0};
                int resultado = recv(jogadores[i].socket, buffer, sizeof(buffer) - 1, 0);

                if (resultado <= 0)
                {
                    desconectar_jogador(i);
                }
                else
                {
                    buffer[resultado] = '\0';
                    printf("Jogador %d: %s\n", jogadores[i].id, buffer);

                    // Verifica se o jogador está pronto para jogar
                    if (strcmp(buffer, "PRONTO_PARA_JOGAR") == 0)
                    {
                        jogadores[i].pronto_para_jogar = 1;
                        printf("Jogador %d está pronto para jogar.\n", jogadores[i].id);
                        verificar_jogadores_prontos();
                    }

                    if (strncmp(buffer, "CARTA_ESCOLHIDA:", 15) == 0)
                    {
                        char *dados = buffer + 15;
                        int jogador_id;
                        char carta[40];
                        sscanf(dados, "%d:%[^:]", &jogador_id, carta);
                        processar_jogada(jogador_id, carta);
                    }

                    if (strcmp(buffer, "INICIAR_JOGO") == 0 && i == 0)
                    {
                        printf("Jogador 1 iniciou o jogo. Iniciando distribuição de cartas...\n");

                        // Resetar o estado do jogo
                        memset(cartas_usadas, 0, sizeof(cartas_usadas));
                        srand(time(NULL)); // Inicializa o gerador de números aleatórios

                        // Primeiro, escolhe a mesa
                        const char *mesas[] = {"Mesa de Reis", "Mesa de Rainhas", "Mesa de Aces"};
                        int mesa_escolhida_index = rand() % 3;
                        strcpy(mesa_escolhida, mesas[mesa_escolhida_index]);

                        // Envia a mesa escolhida para todos os jogadores
                        char mensagem_mesa[BUFFER_TAM];
                        sprintf(mensagem_mesa, "MESA_ESCOLHIDA:%s", mesa_escolhida);

                        for (int j = 0; j < MAX_JOGADORES; j++)
                        {
                            if (jogadores[j].ativo)
                            {
                                enviar_mensagem_para_jogador(j, mensagem_mesa);
                                Sleep(100); // Pequena pausa para evitar sobrecarga
                            }
                        }

                        // Aguarda um momento para que os clientes processem a animação da mesa
                        Sleep(3000);

                        // Depois, distribui as cartas para cada jogador
                        for (int j = 0; j < MAX_JOGADORES; j++)
                        {
                            if (jogadores[j].ativo)
                            {
                                // Distribui 5 cartas para o jogador atual
                                for (int k = 0; k < 5; k++)
                                {
                                    int carta_aleatoria;
                                    do
                                    {
                                        carta_aleatoria = rand() % (sizeof(baralho) / sizeof(baralho[0]));
                                    }
                                    while (cartas_usadas[carta_aleatoria]);

                                    cartas_usadas[carta_aleatoria] = 1;
                                    strcpy(maos_jogadores[j].cartas[k], baralho[carta_aleatoria]);
                                }

                                // Envia as cartas para o jogador
                                char mensagem_cartas[BUFFER_TAM];
                                sprintf(mensagem_cartas, "CARTAS:%s,%s,%s,%s,%s",
                                        maos_jogadores[j].cartas[0],
                                        maos_jogadores[j].cartas[1],
                                        maos_jogadores[j].cartas[2],
                                        maos_jogadores[j].cartas[3],
                                        maos_jogadores[j].cartas[4]);

                                enviar_mensagem_para_jogador(j, mensagem_cartas);
                                Sleep(100); // Pequena pausa para evitar sobrecarga
                            }
                        }

                        printf("Distribuição de cartas concluída.\n");

                        // Registra o início do jogo no log do servidor
                        time_t tempo_atual;
                        time(&tempo_atual);
                        printf("Jogo iniciado em: %s", ctime(&tempo_atual));
                        printf("Mesa escolhida: %s\n", mesa_escolhida);

                        // Você pode adicionar aqui mais lógica para controlar o fluxo do jogo
                        // Por exemplo, definir qual jogador começa, estabelecer turnos, etc.
                    }
                }
            }
        }
    }
}

int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Erro ao inicializar Winsock. Codigo: %d\n", WSAGetLastError());
        return 1;
    }

    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == INVALID_SOCKET)
    {
        printf("Erro ao criar o socket. Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in endereco;
    endereco.sin_family = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port = htons(PORTA);

    if (bind(servidorSocket, (struct sockaddr *)&endereco, sizeof(endereco)) == SOCKET_ERROR)
    {
        printf("Erro no bind. Codigo: %d\n", WSAGetLastError());
        closesocket(servidorSocket);
        WSACleanup();
        return 1;
    }

    if (listen(servidorSocket, MAX_JOGADORES) == SOCKET_ERROR)
    {
        printf("Erro no listen. Codigo: %d\n", WSAGetLastError());
        closesocket(servidorSocket);
        WSACleanup();
        return 1;
    }

    printf("Servidor aguardando jogadores na porta %d...\n", PORTA);
    loop_principal();

    closesocket(servidorSocket);
    WSACleanup();
    return 0;
}
