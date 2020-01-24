/**
 ------------------------------------------------------------
    BIBLIOTECAS
 ------------------------------------------------------------
**/

#include <stdio.h>
#include <stdlib.h>

/**
 ------------------------------------------------------------
    MACROS
 ------------------------------------------------------------
**/

// Caracteres utilizados para representação do jogo
#define XIS         'X'
#define BOLA        'O'
#define NULO        ' '

// Número máximo de elementos
#define MAX         3

// Definição das possiveis situações do jogo
typedef enum
{
    jv_continua = -2,
    jv_player_win = -1,
    jv_bloqueado = 0,
    jv_computador_win = 1,
} jv_state;

/**
 ------------------------------------------------------------
    PROTÓTIPO DE FUNÇÕES
 ------------------------------------------------------------
**/

void LimpaTela(void);
void Imprime(void);
void RecebeJogada(int jogador);
jv_state JogoState(char tabela[MAX][MAX]);
int Minimax(char tabela[MAX][MAX], int jogador);
void ComputerMove(int jogador);

/**
 ------------------------------------------------------------
    VARIÀVEIS GLOBAIS
 ------------------------------------------------------------
**/

// Estrutura de dados utilizada para salvar informações do jogo
char jogo[MAX][MAX];

// Salva caracteres usados para representar os movimentos do jogo
char player, computer;

/**
 ------------------------------------------------------------
    FUNÇÕES
 ------------------------------------------------------------
**/

// Limpa a tela e imprime cabeçalho
void LimpaTela(void)
{
    system("cls");
	
	// Cabeçalho exibido na primeira linha da tela
    printf(" Jogo da Velha:\n");
}

// Imprime o passo atual do jogo na tela
void Imprime(void)
{
    // Variáveis locais
    unsigned int linha, coluna;

    // Limpa a Tela
    LimpaTela();

    // Imprime cabeçalho do jogo
    printf(" Jogador: %c\n", player);
    printf(" Computador: %c\n", computer);
    printf("\n    A   B   C\n");

    // Imprime matriz do jogo
    for (linha = 0; linha < MAX; linha++)
    {
        printf(" %d:", (linha+1));

        for (coluna = 0; coluna < MAX; coluna++)
        {
            printf(" %c ", jogo[linha][coluna]);
            if (coluna != (MAX-1)) printf("|");
        }
        if (linha != (MAX-1)) printf("\n   -----------\n");
    }
}

// Solicita que o jogador informe a sua próxima jogada
void RecebeJogada(int jogador)
{
    // Variáveis locais
    char cl, cc;
    unsigned int linha, coluna;

    if (jogador == -1)
    {
        // Ponto de retorno em caso de erro
        for (;;)
        {
            // Solicita ao operador posição da próxima jogada
            printf("\n\n Jogador: Em qual casa voce gostaria de jogar?\n\n ");
            scanf("%c%c", &cc, &cl);
            fflush(stdin);
            cc = toupper(cc);
            coluna = (unsigned int)(cc - 'A');
            linha = (unsigned int)(cl - '1');

            // Testa erros
            if ((coluna < 0) || (coluna > (MAX-1)) ||
                    (linha < 0) || (linha > (MAX-1)))
                printf("\n Codigo inserido incorreto!");
            else if (jogo[linha][coluna] != NULO)
                printf("\n Esta casa ja se encontra ocupada!");
            else
            {
                // Executa jogada
                jogo[linha][coluna] = player;
                break;
            }
        }
    }
    // Solicita jogada ao computador
    else ComputerMove(jogador);
}

// Testa se o jogo já terminou
jv_state JogoState(char tabela[MAX][MAX])
{
    // Variáveis locais
    unsigned int i, j;
    unsigned int player_cont, computer_cont;
    jv_state state = jv_bloqueado;

    // Tabela de indices para combinações vitoriosas
    static const unsigned int table_l[8][MAX] =
    {
        {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {0, 0, 0},
        {1, 1, 1}, {2, 2, 2}, {0, 1, 2}, {0, 1, 2},
    };
    static const unsigned int table_c[8][MAX] =
    {
        {0, 0, 0}, {1, 1, 1}, {2, 2, 2}, {0, 1, 2},
        {0, 1, 2}, {0, 1, 2}, {0, 1, 2}, {2, 1, 0},
    };

    // Testa todas as condições
    for (i = 0; i < 8; i++)
    {
        // Inicializa contador de elementos
        player_cont = 0;
        computer_cont = 0;

        // Realiza contagem de elementos
        for (j = 0; j < MAX; j++)
        {
            if (tabela[table_l[i][j]][table_c[i][j]] == player)
                player_cont++;
            else if (tabela[table_l[i][j]][table_c[i][j]] == computer)
                computer_cont++;
        }

        // Tomada de decisão
        if (player_cont == MAX) return jv_player_win;
        else if (computer_cont == MAX) return jv_computador_win;
        else if ((player_cont == 0) || (computer_cont == 0))
            state = jv_continua;
    }

    // Retorna condição atual do jogo
    return state;
}

// Algoritmo Minimax
int Minimax(char tabela[MAX][MAX], int jogador)
{
    // Variáveis locais
    unsigned int linha, coluna;
    jv_state score, this_score;

    // Avalia situação atual do jogo
    score = JogoState(tabela);

    /* Se é um nó final, retorna 1 para vitoria
        do jogador e -1 para derota */
    if (score != jv_continua)
        return (jogador*score);
    else
    {
        // Testa todas as condições
        for (coluna = 0; coluna < MAX; coluna++)
        {
            for (linha = 0; linha < MAX; linha++)
            {
                if (tabela[linha][coluna] == NULO)
                {
                    // Se existe jogada possivel, então testa
                    tabela[linha][coluna] = (jogador == 1 ? computer : player);

                    // Encontra melhor jogada para o oponente e a inverte
                    this_score = -Minimax(tabela, -jogador);

                    // Retorna ao valor anterior
                    tabela[linha][coluna] = NULO;

                    // Testa se jogada é eficiente
                    if (this_score > score) score = this_score;
                }
            }
        }
    }

    if (score == jv_continua) return jv_bloqueado;
    return score;
}

// Efetua melhor jogada atraves do algoritmo Minimax
void ComputerMove(int jogador)
{
    // Variáveis locais
    unsigned int linha, coluna;
    unsigned int mov_l, mov_c;
    jv_state score = jv_continua, this_score;

    // Testa todas as jogadas possiveis
    for (coluna = 0; coluna < MAX; coluna++)
    {
        for (linha = 0; linha < MAX; linha++)
        {
            if (jogo[linha][coluna] == NULO)
            {
                // Se existe jogada possivel, então testa
                jogo[linha][coluna] = (jogador == 1 ? computer : player);

                // Encontra melhor jogada para o oponente e a inverte
                this_score = -Minimax(jogo, -jogador);

                // Retorna ao valor anterior
                jogo[linha][coluna] = NULO;

                // Testa se jogada é eficiente
                if (this_score > score)
                {
                    // Salva dados da jogada
                    mov_l = linha;
                    mov_c = coluna;
                    score = this_score;
                }
            }
        }
    }

    // Realiza a jogada
    jogo[mov_l][mov_c] = computer;
}

/**
 ------------------------------------------------------------
    MAIN
 ------------------------------------------------------------
**/

int main ()
{
    // Variáveis locais
    unsigned int i, j;
    unsigned int jogada;
    int jogador;
    jv_state state;
    char aux;

    // Limpa a Tela
    LimpaTela();

    // Fornece informações básicas sobre o jogo
    printf("\n Para informar a casa em que voce deseja jogar, basta informar\n");
    printf(" a letra da coluna, seguido pelo numero da linha, por exemplo: a1!\n");
    printf("\n Bom jogo!\n\n ");

    // Espera o usuário clicar uma tecla do teclado
    system("pause");

    // Limpa a Tela
    LimpaTela();

    // Pergunta qual caracter gostaria de usar
    printf("\n Voce gostaria de jogar usando qual caracter? (X/O)\n\n ");
    scanf("%c", &player);
    fflush(stdin);
    if ((player != XIS) && (player != tolower(XIS))) computer = XIS;
    else computer = BOLA;

    do
    {
        // Inicializa as variaveis
        jogada = 1;
        for (i = 0; i < MAX; i++)
        {
            for (j = 0; j < MAX; j++) jogo[i][j] = NULO;
        }

        // Limpa a Tela
        LimpaTela();

        // Pergunta se o jogador gostaria de jogar por primeiro
        printf("\n Voce gostaria de ser o primeiro a jogar? (Y/N)\n\n ");
        scanf("%c", &aux);
        fflush(stdin);
        aux = toupper(aux);
        if (aux == 'Y') jogador = -1;
        else jogador = 1;

        // Reatualiza a impressão da matriz na tela para o usuário
        Imprime();

        do
        {
            // Solicita para que o usuário efetue uma jogada
            RecebeJogada(jogador);

            // Reatualiza a impressão da matriz na tela para o usuário
            Imprime();

            // Incrementa contador de jogadas
            jogada++;

            // Decide qual jogador executará a próxima jogada
            jogador *= -1;

            // Testa se existe conbinação valida, para então terminar o jogo
            state = JogoState(jogo);
        }
        while (state == jv_continua);

        // Exibe o resultado do jogo
        if (state == jv_player_win)
            printf("\n\n Parabens! Voce ganhou!\n\n ");
        else if (state == jv_computador_win)
            printf("\n\n HAHAHA! Voce perdeu!\n\n ");
        else printf("\n\n O jogo acabou em empate.\n\n ");

        // Espera o usuário clicar uma tecla do teclado
        system("pause");

        // Limpa a Tela
        LimpaTela();

        // Pergunta se o usuario gostaria de jogar novamente
        printf("\n Voce gostaria de jogar novamente? (Y/N)\n\n ");
        scanf("%c", &aux);
        fflush(stdin);
        aux = toupper(aux);
    }
    while (aux == 'Y');

    // Encerra o programa
    return 0;
}

/**
 ------------------------------------------------------------
    FIM
 ------------------------------------------------------------
**/

