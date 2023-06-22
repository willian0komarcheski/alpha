#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>

struct pergunta {
    char tipo;
    char enunciado[150];
    char opcoes[5][30];
    float respN; 
    int codigo; 
    int respO; 
};

struct jogador {
    char nome[16];
    int vida, pontos;
};

struct posicao {
    int coluna, linha;
};

void colisaoavatar(int i, int j, int mapa[10][10], int *pontosdepois, int pontosatual, int *vidadepois, int vidaatual, struct pergunta perguntas[], int tam);
void colisaoatualizar (int i, int j, int mapa[10][10], int *pontosdepois, int pontosatual, int *vidadepois, int vidaatual, struct pergunta perguntas[], int tam);
int randomInteger (int low, int high);
void geraMapa(int mapa[10][10], int nivel);
void atualizaMapa(int mapa[10][10]);
void imprimeMapa(int mapa[10][10], int vida, int pontuacao);
void acharposicao (int *linha, int *coluna, int mapa[10][10]);
void imprimirTodasPerguntas(struct pergunta perguntas[], int tam);
void carregarPerguntasArquivo(struct pergunta perguntas[]);
void inicializarPerguntas(struct pergunta perguntas[], int tam);
int fazerpergunta(struct pergunta perguntas[], int tam);
void imprimeRankingArquivo();
void atualizaRanking(struct jogador ranking[3], int pontuacaoAtual, char nomeAtual[15]);
void obtemRanking(struct jogador ranking[3]);
void gravaRanking(struct jogador ranking[3]);
int fimescolha();
int saiudomapa(int vida);
void fimrank(struct jogador jogador, struct jogador ranking[3]);
void derrota(struct jogador jogador, struct jogador ranking[3]);
void vitoria(struct jogador jogador, struct jogador ranking[3]);

void fimrank(struct jogador jogador, struct jogador ranking[3])
 	{
 	atualizaRanking(ranking, jogador.pontos, jogador.nome);
 	imprimeRankingArquivo();
	}

void derrota(struct jogador jogador, struct jogador ranking[3])
	{
	int a;
	printf("\n\n\n\n\n\n\n\nque pena %s, você reprovou em calculo, mas pelo menos suas tentaivas gerou alguns pontos, sua pontuação é: %d\n",jogador.nome ,jogador.pontos);
	printf("\n o ranking dos 3 jogadores que ganharam mais pontos ira aparecer a seguir");
	fimrank(jogador, ranking);
	for(a = 0; a <=3; a++)
		{
		if(jogador.pontos == ranking[a].pontos)
			{
			break;
			};
		};
	if(a != 3)
		{
		printf("\n\nparabens %s, você conseguiu entrar no top 03 pessoas com maior pontuação, sua pontuação é:%d\n",jogador.nome , jogador.pontos);
		}
	}

void vitoria(struct jogador jogador, struct jogador ranking[3])
	{
	int a;
	printf("\n\n\n\n\n\n\n\nparabens %s, você conseguiu a media para passar em calculo, com %d provas sendo acima da media, e com uma pontuação de: %d\n", jogador.nome , jogador.vida , jogador.pontos);
	printf("\n o ranking dos 3 jogadores que ganharam mais pontos ira aparecer a seguir");
	fimrank(jogador, ranking);
	for(a = 0; a <=3; a++)
		{
		if(jogador.pontos == ranking[a].pontos)
			{
			break;
			};
		};
	if(a != 3)
		{
		printf("\n\nparabens %s, você conseguiu entrar no top 03 pessoas com maior pontuação, sua pontuação é: %d\n", jogador.nome , jogador.pontos);
		}
	}

void inicializarPerguntas(struct pergunta perguntas[], int tam) {
    int i;
    for(i=0; i<tam; i++) {
        perguntas[i].codigo=-1;
    }
}

void carregarPerguntasArquivo(struct pergunta perguntas[]) {
    FILE *arq = fopen("pergs.txt", "r");
    int linha=0, parte;
    char frase[1000];

    while (fscanf(arq, "%[^\n] ", frase) != EOF){
        char *token = strtok(frase, ";");
        parte=0;
        while(token != NULL) {
            if (parte == 0)
                perguntas[linha].codigo = atoi(token);
            else if (parte == 1)
                perguntas[linha].tipo = token[0];
            else if (parte == 2)
                strcpy(perguntas[linha].enunciado, token);
            else if (parte == 3 && perguntas[linha].tipo == 'N') {
                perguntas[linha].respN = atof(token);
            } else if (parte == 3 && perguntas[linha].tipo == 'O') {
                perguntas[linha].respO = atoi(token);
            } else if (parte ==4)
                strcpy(perguntas[linha].opcoes[0], token);
            else if (parte ==5)
                strcpy(perguntas[linha].opcoes[1], token);
            else if (parte ==6)
                strcpy(perguntas[linha].opcoes[2], token);
            else if (parte ==7)
                strcpy(perguntas[linha].opcoes[3], token);
            else if (parte ==8)
                strcpy(perguntas[linha].opcoes[4], token);

            token = strtok(NULL, ";");
            parte++;
        }
        linha++;
    };
}

void imprimirTodasPerguntas(struct pergunta perguntas[], int tam) {
    int i,j;
    for(i=0; i<tam; i++) {
        if (perguntas[i].codigo < 0) break;
        printf("\n\nPERGUNTA: %d -- %s", perguntas[i].codigo, perguntas[i].enunciado);
        if (perguntas[i].tipo == 'N') {
            printf("\n\tTipo: Numï¿½rica");
            printf("\n\tResposta: %.1f", perguntas[i].respN);
        } else {
            printf("\n\tTipo: Objetiva");
            for(j=0; j<5; j++) {
                printf("\n\tOpï¿½ï¿½o %d: %s", j+1, perguntas[i].opcoes[j]);
            }
            printf("\n\tResposta (opï¿½ï¿½o): %d", perguntas[i].respO);
        }
    }
}

void colisaoavatar(int i, int j, int mapa[10][10], int *pontosdepois, int pontosatual, int *vidadepois, int vidaatual, struct pergunta perguntas[], int tam)
    {
    switch (mapa[i][j])
                        {
                        case 0:
                            mapa[i][j] = 1;
                            break;
                        case 3:
                            mapa[i][j] = 1;
                            if (fazerpergunta(perguntas, tam))
                                {
                                pontosatual = pontosatual + 500;
                                }
                            else
                                {
                                vidaatual = vidaatual - 1;
                                };
                            break;
                        case 4:
                            mapa[i][j] = 1;
                            pontosatual = pontosatual + 1000;
                            break;
                        }
    *vidadepois = vidaatual;
    *pontosdepois = pontosatual;
    }

void colisaoatualizar (int i, int j, int mapa[10][10], int *pontosdepois, int pontosatual, int *vidadepois, int vidaatual, struct pergunta perguntas[], int tam)
    {
    switch (mapa[i][j])
                        {
                        case 3:
                            mapa[i][j] = 1;
                            if (fazerpergunta(perguntas, tam))
                                {
                                pontosatual = pontosatual + 500;
                                }
                            else
                                {
                                vidaatual = vidaatual - 1;
                                };
                            break;
                        case 4:
                            mapa[i][j] = 1;
                            pontosatual = pontosatual + 1000;
                            break;
                        }
    *vidadepois = vidaatual;
    *pontosdepois = pontosatual;
    }

int saiudomapa(int vida)
    {
    printf("\n\nvocÃª saiu da faculdade e se distraiu e foi parar em um bar, ficou bebado e acordou atrasado no outro dia e perdeu a prova.\n\n");
    return vida -1;
    }

int fimescolha()
    {
    int a;
    int b;
    printf("\n\n\n\n\n\n\n\n");
    do
        {
        printf("\n\n\n\n\n\n                                                                    JOGAR NOVAMENTE(1)\n                                                                    ENCERRAR JOGO(0)\n\n\n\n\n\n\n\n\n\n");
        a = getch();
        printf("\n                                                                    TEM CERTEZA?\n                                                                    SIM(1)\n                                                                    NÃƒO(0)\n\n\n\n\n\n\n\n");
        b = getch();
        } while(b != '1');
    if(a == '1')
        {
        return 1;
        }
    else
        {
        return 0;
        };
    }

int fazerpergunta(struct pergunta perguntas[], int tam)
    {
    int a, c;
    float b;
    
    printf("\n\n\n\n\n\n\n\nvocÃª se esbarrou em um professor, agora vocÃª tera que acertar a pergunta dele ou ele vai zerar sua prova, mas se vocÃª acertar a pergunta ele lhe dara nota extra.\n\n");
    do {
        a = randomInteger(0, tam);
    } while(perguntas[a].codigo == -1);
    if (perguntas[a].tipo == 'N')
        {
        printf("\n%s\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", perguntas[a].enunciado);
        scanf("%f", &b);
        if (b == perguntas[a].respN)
            {
            printf("\nparabens, vocÃª acertou.\n");
            return 1;
            }
        else
            {
            printf("\nque pena, vocÃª errou.\n");
            return 0;
            };
        }
    else
        {
        printf("\n%s\n", perguntas[a].enunciado);
        printf("\n%c\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", perguntas[a].opcoes);
        scanf("%d", &c);
        if (c == perguntas[a].respO)
            {
            printf("\nparabens, vocÃª acertou.\n");
            return 1;
            }
        else
            {
            printf("\nque pena, vocÃª errou.\n");
            return 0;
            };
        };
    perguntas[a].codigo = -1;
    }

void obtemRanking(struct jogador ranking[3]) {
    FILE *arq = fopen("rank.dat", "rb");
    int total_lido;
    if (arq == NULL) {
        ranking[0].pontos=0;
        ranking[1].pontos=0;
        ranking[2].pontos=0;
        strcpy(ranking[0].nome, "");
        strcpy(ranking[1].nome, "");
        strcpy(ranking[2].nome, "");
    } else {
        total_lido = fread(ranking, sizeof(struct jogador), 3, arq);
        if (total_lido == 3) {
            printf(".");
        } else
            printf("Erro na gravaÃ§Ã£o dos dados");
    }
    fclose(arq);
}


void gravaRanking(struct jogador ranking[3]) {
    FILE *arq = fopen("rank.dat", "wb");
    int total_gravado;
    if (arq == NULL)
        printf("Erro na abertura do arquivo.");
    else {
        total_gravado = fwrite(ranking, sizeof(struct jogador), 3, arq);
        if (total_gravado == 3) {
            printf(".");
        } else
            printf("Erro na gravacÃ£o dos dados");
    }
    fclose(arq);
}

void atualizaRanking(struct jogador ranking[3], int pontuacaoAtual, char nomeAtual[15]) {
    struct jogador aux;

    if (pontuacaoAtual > ranking[2].pontos) {
        ranking[2].pontos = pontuacaoAtual;
        strcpy(ranking[2].nome, nomeAtual);
        if (pontuacaoAtual > ranking[1].pontos) {
            aux.pontos = ranking[1].pontos;
            strcpy(aux.nome, ranking[1].nome);

            ranking[1].pontos = ranking[2].pontos;
            strcpy(ranking[1].nome, ranking[2].nome);

            ranking[2].pontos = aux.pontos;
            strcpy(ranking[2].nome, aux.nome);

            if (pontuacaoAtual > ranking[0].pontos) {
                aux.pontos = ranking[0].pontos;
                strcpy(aux.nome, ranking[0].nome);

                ranking[0].pontos = ranking[1].pontos;
                strcpy(ranking[0].nome, ranking[1].nome);

                ranking[1].pontos = aux.pontos;
                strcpy(ranking[1].nome, aux.nome);
            }
        }
        gravaRanking(ranking);
    }
}

void imprimeRankingArquivo() {
    struct jogador ranking[3];
    int total_lido;
    FILE *arq = fopen("rank.dat", "rb");
    if (arq == NULL) {
        printf("Erro");
    } else {
        total_lido = fread(ranking, sizeof(struct jogador), 3, arq);
        if (total_lido == 3) {
            printf(".");

            printf("\n\n %s --- %d", ranking[0].nome, ranking[0].pontos);
            printf("\n\n %s --- %d", ranking[1].nome, ranking[1].pontos);
            printf("\n\n %s --- %d", ranking[2].nome, ranking[2].pontos);

        } else
            printf("Erro na gravaÃ§Ã£o dos dados");
    }
    fclose(arq);

}

int randomInteger (int low, int high) {
    int k;
    double d;

    d = (double) rand () / ((double) RAND_MAX + 1);
    k = d * (high - low + 1);

    return low + k;
}

void geraMapa(int mapa[10][10], int nivel) {
    int i, j, qtdO, qtdA, lin, col, infO=5, supO=15;

    switch (nivel) {
        case 2: infO=5; supO=25; break;
        case 3: infO=15; supO=35; break;
    }

    srand((unsigned int)time(NULL));

    for (i=0; i<10; i++)
        for (j=0; j<10; j++)
            mapa[i][j] = 0;

    mapa[randomInteger(0,9)][0]= 1;
    mapa[randomInteger(0,9)][9]= 2;
    qtdA = randomInteger(3,10);
    qtdO = randomInteger(infO, supO);

    while (qtdO>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 3;
            qtdO--;
        }
    }

    while (qtdA>0) {
        lin = randomInteger(0,9);
        col = randomInteger(1,8);
        if (mapa[lin][col] == 0) {
            mapa[lin][col] = 4;
            qtdA--;
        }
    }
}

void atualizaMapa(int mapa[10][10]) {
    int i, j, upd, atualizado[10][10];
    for (i=0; i<10; i++) for (j=0; j<10; j++) atualizado[i][j]=0;

    for (i=0; i<10; i++)
        for (j=0; j<10; j++) {
            if ((mapa[i][j] == 2 || mapa[i][j] == 3 || mapa[i][j] == 4) && atualizado[i][j] == 0) {
                upd = randomInteger(1,4);
                switch (upd) {
                    case 1:
                        if ((mapa[i-1][j] == 0 || (mapa[i-1][j] == 1 && mapa[i][j] == 3)) && i>0) {
                            mapa[i-1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i-1][j]=1;
                        }
                        break;
                    case 2:
                        if ((mapa[i+1][j] == 0 || (mapa[i+1][j] == 1 && mapa[i][j] == 3)) && i<9) {
                            mapa[i+1][j] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i+1][j]=1;
                        }
                        break;
                    case 3:
                        if ((mapa[i][j+1] == 0 || (mapa[i][j+1] == 1 && mapa[i][j] == 3)) && j<9) {
                            mapa[i][j+1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j+1]=1;
                        }
                        break;
                    case 4:
                        if ((mapa[i][j-1] == 0 || (mapa[i][j-1] == 1 && mapa[i][j] == 3)) && j>0) {
                            mapa[i][j-1] = mapa[i][j];
                            mapa[i][j] = 0;
                            atualizado[i][j-1]=1;
                        }
                }
            }
       }
}

void imprimeMapa(int mapa[10][10], int vida, int pontuacao)
{
    int i, j;
/*    ClearScreen();*/
    if (vida >= 1)
        {
        printf("                                                                 VIDA: S2");
        };
        if (vida >= 2)
            {
            printf(" S2");
            };
            if(vida >= 3)
                {
                printf(" S2");
                };
    printf("\n                                                                 PONTUAÃ‡Ã‚O:%d", pontuacao);
    printf("\n                                                                  ________________________________________\n");
    for (i=0; i<10; i++) {
        printf("                                                                 |");
        for (j=0; j<10; j++) {
            if (mapa[i][j]==0)
                printf("[  ]");
            else
                if (mapa[i][j]==1)
                    {
                     printf("[ X]");
                    }
                else
                    {
                    if (mapa[i][j]==3)
                        {
                        printf("[=)]");
                        }
                    else
                        {
                        if(mapa[i][j] == 4)
                            {
                            printf("[:D]");
                            }
                        else
                            {
                            if(mapa[i][j] == 2)
                                {
                                printf("[O ]");
                                };
                            };
                        };
                    };
        }
        printf("|\n");
    }
printf("                                                                  _________________________________________\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void acharposicao (int *linha, int *coluna, int mapa[10][10])
    {
    int i, j;
    for(i = 0; i < 9; i++)
        {
        for(j = 0; j < 9; j++)
            {
            if(mapa[i][j] == 1)
                {
                break;
                }
            }
        if(mapa[i][j] == 1)
            {
            break;
            }
        }
    *coluna = j;
    *linha = i;
    }

int main()
    {
    setlocale(LC_ALL, "Portuguese");
    int mapa[10][10], ch, a, tam = 30;
    struct posicao posicao;
    struct jogador jogador;
    struct jogador ranking[3];
    obtemRanking(ranking);
    struct pergunta perguntas[tam];
    inicializarPerguntas(perguntas, tam);
    carregarPerguntasArquivo(perguntas);
    /*imprimirTodasPerguntas(perguntas,tam);*/
    printf("\n                                                                                                                                 //desenvolvido por: alunos rancorosos//\n\n\n\n\n\n\n BEM VINDO AO:\n\n\n\n\n");
    printf("      A        L      U     U  NN     N     OO       DD     EEEEEEEE       CCCCCC        A        L           CCC   U      U  L           OO     \n");
    printf("     A A       L      U     U  N N    N    O  O      D D    E             C             A A       L          C      U      U  L          O   O   \n");
    printf("    A   A      L      U     U  N  N   N   O    0     D  D   E            C             A   A      L         C       U      U  L         O     O  \n");
    printf("   AAAAAAA     L      U     U  N   N  N  O      0    D   D  EEEEEEEE    C             AAAAAAA     L        C        U      U  L        O      O  \n");
    printf("  A       A    L      U     U  N    N N   0    0     D  D   E            C           A       A    L         C       U      U  L         O    O   \n");
    printf(" A         A   L      U     U  N     NN    0  0      D D    E             C         A         A   L          C      U      U  L          O  O    \n");
    printf("A           A  LLLLL  UUUUUUU  N      N     0O       DD     EEEEEEEE       CCCCCC  A           A  LLLLLLLL    CCCC  UUUUUUUU  LLLLLLLL    OO      .SIMULATOR\n\n\n\n\n");
    do
        {
        printf("\n\n\n\n\n agora, digite seu codinome: ");
        gets(jogador.nome);
        for(a = 0; a < 15; a++)
            {
            if (jogador.nome[a] == '\0')
                {
                break;
                }
            };
        if(a < 3 || a == 15)
            {
            printf("\nvocÃª digitou um codinome muito grande ou muito pequeno");
            };
        } while(a < 3 || a == 15);
    printf("\n\n\n\n\n\nbem vindo ao ALUNO DE CALCUlO SIMULATOR %s, nesse jogo você sera um estudante de calculo tentando passar na materia, ocorrerão eventos onde você podera perder nota(vida), mas esses eventos sempre vem com recompensas(pontos), \nas regras serão:\nvocê sera um X no mapa\no X tem que chegar no O para passar em calculo\nterao obstaculos(professores), no caminho que poderão tirar sua nota(vida), nas provas\nvocê começara com 3 vidas e 0 pontos\ntera obstaculos que em vez de tirar sua nota(vida), lhe dara pontos extras\nos obstaculos se movimentam, tantos os malignos(professores), quanto os benignos(RU).\nse você perder suas 3 vidas, você perde o jogo, mas ainda fica legivel para entrar no ranking de top 3 maiores ganhadores de pontos. \n", jogador.nome);
	do
		{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                         NOVO JOGO(1)\n                                                         MOVIMENTAÇÃO(2)\n                                                         ENCERRAR JOGO(0)\n\n\n\n\n\n");	
		a = getch();
		if (a == '2') 
			{
			printf("\naki você se movimenta-ra com as teclas : w,a,s,d, sendo w pra cima, a pra esquerda, s para baixo e d para direita\n");
			}
	} while(a != '1' && a != '0');
	while(1)
        {
       	if (a == '0') 
			{
			break;
			}
        jogador.vida = 3;
        jogador.pontos = 0;
        geraMapa(mapa, 3);
        acharposicao(&posicao.linha , &posicao.coluna , mapa);
        while ( jogador.vida > 0 && mapa[posicao.linha][posicao.coluna] != 2 )
            {
            imprimeMapa(mapa, jogador.vida, jogador.pontos);
            switch ( getch() )
                {
                case 'w':
                    mapa[posicao.linha][posicao.coluna] = 0;
                    posicao.linha = posicao.linha - 1;
                    if ((posicao.linha < 0 || posicao.coluna < 0) || (posicao.linha > 9 || posicao.coluna > 9))
                        {
                        jogador.vida = saiudomapa(jogador.vida);
                        posicao.linha = posicao.linha + 1;
                        mapa[posicao.linha][posicao.coluna] = 1;
                        }
                    else
                        {
                        colisaoavatar (posicao.linha , posicao.coluna , mapa , &jogador.pontos, jogador.pontos, &jogador.vida, jogador.vida, perguntas, tam);
                        };
                    break;
                case 's':
                    mapa[posicao.linha][posicao.coluna] = 0;
                    posicao.linha = posicao.linha + 1;
                    if ((posicao.linha < 0 || posicao.coluna < 0) || (posicao.linha > 9 || posicao.coluna > 9) )
                        {
                        jogador.vida = saiudomapa(jogador.vida);
                        posicao.linha = posicao.linha - 1;
                        mapa[posicao.linha][posicao.coluna] = 1;
                        }
                    else
                        {
                        colisaoavatar (posicao.linha , posicao.coluna , mapa , &jogador.pontos, jogador.pontos, &jogador.vida, jogador.vida, perguntas, tam);
                        };
                    break;
                case 'a':
                    mapa[posicao.linha][posicao.coluna] = 0;
                    posicao.coluna = posicao.coluna - 1;
                    if ((posicao.linha < 0 || posicao.coluna < 0) || (posicao.linha > 9 || posicao.coluna > 9))
                        {
                        jogador.vida = saiudomapa(jogador.vida);
                        posicao.coluna = posicao.coluna + 1;
                        mapa[posicao.linha][posicao.coluna] = 1;
                        }
                    else
                        {
                        colisaoavatar (posicao.linha , posicao.coluna , mapa, &jogador.pontos, jogador.pontos, &jogador.vida, jogador.vida, perguntas, tam);
                        };
                    break;
                case 'd':
                    mapa[posicao.linha][posicao.coluna] = 0;
                    posicao.coluna = posicao.coluna + 1;
                    if ((posicao.linha < 0 || posicao.coluna < 0) || (posicao.linha > 9 || posicao.coluna > 9))
                        {
                        jogador.vida = saiudomapa(jogador.vida);
                        posicao.coluna = posicao.coluna - 1;
                        mapa[posicao.linha][posicao.coluna] = 1;
                        }
                    else
                        {
                        colisaoavatar (posicao.linha , posicao.coluna , mapa, &jogador.pontos, jogador.pontos, &jogador.vida, jogador.vida, perguntas, tam);
                        };
                    break;
                }
            if (mapa[posicao.linha][posicao.coluna] != 2)
                {
                atualizaMapa(mapa);
                colisaoatualizar (posicao.linha, posicao.coluna, mapa, &jogador.pontos, jogador.pontos, &jogador.vida, jogador.vida, perguntas, tam);
                };
            }
        if(jogador.vida == 0)
            {
            derrota(jogador, ranking);
            }
        else
            {
            vitoria(jogador, ranking);
            };
        if(fimescolha())
            {
           continue;
            }
        else
            {
            break;
            };
        }
    return 0;
    }
