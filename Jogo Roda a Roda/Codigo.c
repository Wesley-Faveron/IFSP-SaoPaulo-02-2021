#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <dos.h>
#include <windows.h>

#define MAXCARC 17
#define MAXPAL 3

typedef struct
{
    char Pista[MAXCARC];
    int Qtd;
    char Palavras[MAXPAL][MAXCARC];
} dados;

typedef struct
{
    char nome[MAXCARC];
    int valor;
    int jogada;
    int verificador;
} jogador;

FILE *arquivo_leitura;
jogador j[3];

void lista_operacoes();
void imprimir_banco_de_dados();
void adicionar_ao_banco_de_dados();
void sortear_palavra(char Pi[], char Pa[][MAXCARC], int *k);
float sortear_valor_letra();
void exibir_tela(char Pi[], int n, char Pa[][MAXCARC], int comp[][MAXCARC]);
int compara_palavras(char Ori[][MAXCARC], char Dig[][MAXCARC], int n);
int verifica_senha();
void contagem_regressiva();
void som_catraca();
int letra_escolhida(char L, char letras[26]);

int main()
{
    setlocale(LC_ALL, "Portuguese");

    char codigo, letra, vet_letras[26] = "";
    int i, k, Qtd_da_rodada, letras_faltando, cont, tentativas;
    float valor;

    char Pista_da_rodada[MAXCARC], Palavras_da_rodada[MAXPAL][MAXCARC], Palavra[MAXPAL][MAXCARC];
    int Matriz_de_comparacao[MAXPAL][MAXCARC];

    printf("BEM-VIMDO ao jogo Roda a Roda\n");
    lista_operacoes();
    fflush(stdin); codigo = getchar();

    while (codigo != 'f')
    {
        switch (codigo)
        {
            case 'j' :
                memset(Pista_da_rodada, 0, MAXCARC);
                memset(vet_letras, 0, 26);
                Qtd_da_rodada = letras_faltando = tentativas = 0;
                for(i = 0; i < MAXPAL; i++)
                {
                    memset(Palavras_da_rodada[i], 0, MAXCARC);
                    memset(Palavra[i], 0, MAXCARC);
                }
                for(i = 0; i < MAXPAL; i++)
                    for(k = 0; k < MAXCARC; k++)
                        Matriz_de_comparacao[i][k] = 0;

                printf("\nOlá vamos iniciar! Por favor insira o nome de cada jogador\n");
                for(i = 0; i < 3; i++)
                {
                    printf("\nQual o nome do jogador %d: ", i+1);
                    fflush(stdin); scanf("%s", &j[i].nome);
                    j[i].valor = j[i].jogada = j[i].verificador = 0;
                }

                sortear_palavra(Pista_da_rodada, Palavras_da_rodada, &Qtd_da_rodada);

                for (i = 0; i < Qtd_da_rodada; i++)
                    for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                        if (Palavras_da_rodada[i][k] != '\0')
                        {
                            Matriz_de_comparacao[i][k] = 1;
                            letras_faltando ++;
                        }

                j[0].jogada = 1;

                while(letras_faltando > 0)
                {
                    if (j[0].jogada == 1 && j[0].verificador == 0)
                    {
                        exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                        if(letras_faltando > 3)
                        {
                            printf("\n\nRoda a roda! %s\n", j[0].nome);
                            getch();
                            valor = sortear_valor_letra();

                            if(valor == 0)
                            {
                               printf("\nPASSA A VEZ\n");
                               printf("\nA vez será passada para %s\n", j[1].nome);
                               j[0].jogada = 0;
                               j[1].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    printf("\nPERDEU TUDO\n");
                                    printf("\nA vez será passada para %s\n", j[1].nome);
                                    j[0].valor = j[0].jogada = 0;
                                    j[1].jogada = 1;
                                }
                                else
                                {
                                    do
                                    {
                                        printf("\nUma letra por R$ %.0f,00: ", valor);
                                        fflush(stdin); letra = getchar();
                                        letra = toupper(letra);
                                    }while(letra_escolhida(letra, vet_letras) != 0 );

                                    cont = 0;
                                    for (i = 0; i < Qtd_da_rodada; i++)
                                        for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                            if (Palavras_da_rodada[i][k] == letra)
                                            {
                                                Matriz_de_comparacao[i][k] = 0;
                                                j[0].valor += valor;
                                                cont ++;
                                            }
                                    if(cont == 0)
                                    {
                                        printf("\nNão existe letra %c na palavra.\n", letra);
                                        printf("\nA vez será passada para %s\n", j[1].nome);
                                        j[0].jogada = 0;
                                        j[1].jogada = 1;
                                    }
                                    letras_faltando -= cont;
                                }
                            }
                        }
                        else
                        {
                            printf("\n\nRoda a roda! %s\n", j[0].nome);
                            getch();
                            valor = sortear_valor_letra();
                            j[0].verificador = 1;
                            if (valor == 0)
                            {
                                tentativas++;
                                printf("\nPASSA A VEZ\n");
                                if (tentativas < 3)
                                    printf("\nA vez será passada para %s\n", j[1].nome);
                                j[0].jogada = 0;
                                j[1].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    tentativas++;
                                    printf("\nPERDEU TUDO\n");
                                    if (tentativas < 3)
                                        printf("\nA vez será passada para %s\n", j[1].nome);
                                    j[0].valor = j[0].jogada = 0;
                                    j[1].jogada = 1;
                                }
                                else
                                {
                                    if (tentativas < 3)
                                    {
                                        tentativas++;
                                        printf("\n%s: Faltam %d letras para completar a(s) palavra(s)\n", j[0].nome, letras_faltando);
                                        contagem_regressiva();
                                        printf("\nValendo R$ %.0f,00, a(s) palavra(s) é(são)", j[0].valor + valor);
                                        for (i = 0; i < Qtd_da_rodada; i++)
                                        {
                                            printf("\n%d° palavra: ", i+1);
                                            fflush(stdin); scanf("%s", &Palavra[i]);
                                        }
                                        if(compara_palavras(Palavras_da_rodada, Palavra, Qtd_da_rodada) == 0)
                                        {
                                            j[0].valor += valor;
                                            for (i = 0; i < Qtd_da_rodada; i++)
                                                for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                                        Matriz_de_comparacao[i][k] = 0;
                                            exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                                            printf("\nPARABÉNS %s você ganhou R$ %d,00 !!!!!\n\n", j[0].nome, j[0].valor);
                                            letras_faltando = j[0].jogada = j[1].jogada = j[2].jogada = 0;
                                        }
                                        else
                                        {
                                            printf("\nQue pena %s você errou\n", j[0].nome);
                                            if (tentativas < 3)
                                                printf("\nA vez será passada para %s\n", j[1].nome);
                                            j[0].jogada = 0;
                                            j[1].jogada = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (j[1].jogada == 1 && j[1].verificador == 0)
                    {
                        exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                        if(letras_faltando > 3)
                        {
                            printf("\n\nRoda a roda! %s\n", j[1].nome);
                            getch();
                            valor = sortear_valor_letra();

                            if(valor == 0)
                            {
                               printf("\nPASSA A VEZ\n");
                               printf("\nA vez será passada para %s\n", j[2].nome);
                               j[1].jogada = 0;
                               j[2].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    printf("\nPERDEU TUDO\n");
                                    printf("\nA vez será passada para %s\n", j[2].nome);
                                    j[1].valor = j[1].jogada = 0;
                                    j[2].jogada = 1;
                                }
                                else
                                {
                                    do
                                    {
                                        printf("\nUma letra por R$ %.0f,00: ", valor);
                                        fflush(stdin); letra = getchar();
                                        letra = toupper(letra);
                                    }while(letra_escolhida(letra, vet_letras) != 0 );
                                    cont = 0;
                                    for (i = 0; i < Qtd_da_rodada; i++)
                                        for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                            if (Palavras_da_rodada[i][k] == letra)
                                            {
                                                Matriz_de_comparacao[i][k] = 0;
                                                j[1].valor += valor;
                                                cont ++;
                                            }
                                    if(cont == 0)
                                    {
                                        printf("\nNão existe letra %c na palavra.\n", letra);
                                        printf("\nA vez será passada para %s\n", j[2].nome);
                                        j[1].jogada = 0;
                                        j[2].jogada = 1;
                                    }
                                    letras_faltando -= cont;
                                }
                            }
                        }
                        else
                        {
                            printf("\n\nRoda a roda! %s\n", j[1].nome);
                            getch();
                            valor = sortear_valor_letra();
                            j[1].verificador = 1;
                            if (valor == 0)
                            {
                                tentativas++;
                                printf("\nPASSA A VEZ\n");
                                if (tentativas < 3)
                                    printf("\nA vez será passada para %s\n", j[2].nome);
                                j[1].jogada = 0;
                                j[2].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    tentativas++;
                                    printf("\nPERDEU TUDO\n");
                                    if (tentativas < 3)
                                        printf("\nA vez será passada para %s\n", j[2].nome);
                                    j[1].valor = j[1].jogada = 0;
                                    j[2].jogada = 1;
                                }
                                else
                                {
                                    if (tentativas < 3)
                                    {
                                        tentativas++;
                                        printf("\n%s: Faltam %d letras para completar a(s) palavra(s)\n", j[1].nome, letras_faltando);
                                        contagem_regressiva();
                                        printf("\nValendo R$ %.0f,00, a(s) palavra(s) é(são)", j[1].valor + valor);
                                        for (i = 0; i < Qtd_da_rodada; i++)
                                        {
                                            printf("\n%d° palavra: ", i+1);
                                            fflush(stdin); scanf("%s", &Palavra[i]);
                                        }
                                        if(compara_palavras(Palavras_da_rodada, Palavra, Qtd_da_rodada) == 0)
                                        {
                                            j[1].valor += valor;
                                            for (i = 0; i < Qtd_da_rodada; i++)
                                                for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                                        Matriz_de_comparacao[i][k] = 0;
                                            exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                                            printf("\nPARABÉNS %s você ganhou R$ %d,00 !!!!!\n\n", j[1].nome, j[1].valor);
                                            letras_faltando = j[0].jogada = j[1].jogada = j[2].jogada = 0;
                                        }
                                        else
                                        {
                                            printf("\nQue pena %s você errou\n", j[1].nome);
                                            if (tentativas < 3)
                                                printf("\nA vez será passada para %s\n", j[2].nome);
                                            j[1].jogada = 0;
                                            j[2].jogada = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (j[2].jogada == 1 && j[2].verificador == 0)
                    {
                        exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                        if(letras_faltando > 3)
                        {
                            printf("\n\nRoda a roda! %s\n", j[2].nome);
                            getch();
                            valor = sortear_valor_letra();

                            if(valor == 0)
                            {
                               printf("\nPASSA A VEZ\n");
                               printf("\nA vez será passada para %s\n", j[0].nome);
                               j[2].jogada = 0;
                               j[0].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    printf("\nPERDEU TUDO\n");
                                    printf("\nA vez será passada para %s\n", j[0].nome);
                                    j[2].valor = j[2].jogada = 0;
                                    j[0].jogada = 1;
                                }
                                else
                                {
                                    do
                                    {
                                        printf("\nUma letra por R$ %.0f,00: ", valor);
                                        fflush(stdin); letra = getchar();
                                        letra = toupper(letra);
                                    }while(letra_escolhida(letra, vet_letras) != 0 );
                                    cont = 0;
                                    for (i = 0; i < Qtd_da_rodada; i++)
                                        for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                            if (Palavras_da_rodada[i][k] == letra)
                                            {
                                                Matriz_de_comparacao[i][k] = 0;
                                                j[2].valor += valor;
                                                cont ++;
                                            }
                                    if(cont == 0)
                                    {
                                        printf("\nNão existe letra %c na palavra.\n", letra);
                                        printf("\nA vez será passada para %s\n", j[0].nome);
                                        j[2].jogada = 0;
                                        j[0].jogada = 1;
                                    }
                                    letras_faltando -= cont;
                                }
                            }
                        }
                        else
                        {
                            printf("\n\nRoda a roda! %s\n", j[2].nome);
                            getch();
                            valor = sortear_valor_letra();
                            j[2].verificador = 1;
                            if (valor == 0)
                            {
                                tentativas++;
                                printf("\nPASSA A VEZ\n");
                                if (tentativas < 3)
                                    printf("\nA vez será passada para %s\n", j[0].nome);
                                j[2].jogada = 0;
                                j[0].jogada = 1;
                            }
                            else
                            {
                                if(valor > 0 && valor < 0.02)
                                {
                                    tentativas++;
                                    printf("\nPERDEU TUDO\n");
                                    if (tentativas < 3)
                                        printf("\nA vez será passada para %s\n", j[0].nome);
                                    j[2].valor = j[2].jogada = 0;
                                    j[0].jogada = 1;
                                }
                                else
                                {
                                    if (tentativas < 3)
                                    {
                                        tentativas++;
                                        printf("\n%s: Faltam %d letras para completar a(s) palavra(s)\n", j[2].nome, letras_faltando);
                                        contagem_regressiva();
                                        printf("\nValendo R$ %.0f,00, a(s) palavra(s) é(são)", j[2].valor + valor);
                                        for (i = 0; i < Qtd_da_rodada; i++)
                                        {
                                            printf("\n%d° palavra: ", i+1);
                                            fflush(stdin); scanf("%s", &Palavra[i]);
                                        }
                                        if(compara_palavras(Palavras_da_rodada, Palavra, Qtd_da_rodada) == 0)
                                        {
                                            j[2].valor += valor;
                                            for (i = 0; i < Qtd_da_rodada; i++)
                                                for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                                        Matriz_de_comparacao[i][k] = 0;
                                            exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                                            printf("\nPARABÉNS %s você ganhou R$ %d,00 !!!!!\n\n", j[2].nome, j[2].valor);
                                            letras_faltando = j[0].jogada = j[1].jogada = j[2].jogada = 0;
                                        }
                                        else
                                        {
                                            printf("\nQue pena %s você errou\n", j[2].nome);
                                            if (tentativas < 3)
                                                printf("\nA vez será passada para %s\n", j[0].nome);
                                            j[2].jogada = 0;
                                            j[0].jogada = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if (tentativas > 2 && letras_faltando != 0)
                    {
                        for (i = 0; i < Qtd_da_rodada; i++)
                            for (k = 0; k < strlen(Palavras_da_rodada[i]); k++)
                                Matriz_de_comparacao[i][k] = 0;
                        exibir_tela(Pista_da_rodada, Qtd_da_rodada, Palavras_da_rodada, Matriz_de_comparacao);
                        printf("\nInfelizmente não houve ganhador\n");
                        letras_faltando = j[0].jogada = j[1].jogada = j[2].jogada = 0;
                    }
                }
                break;

            case 'a' :
                if (verifica_senha() == 1)
                    adicionar_ao_banco_de_dados();
                else
                    printf("\nSenha inválida.\n");
                break;

            case 'v' :
                if (verifica_senha() == 1)
                    imprimir_banco_de_dados();
                else
                    printf("\nSenha inválida.\n");
                break;

            default  :
                printf("\nCódigo ( %c )inválido.\n", codigo);
        }
        for(i = 0; i < 114 ; i++)
            printf("-");
        printf("\n");
        lista_operacoes();
        fflush(stdin); codigo = getchar();
    }
    return 0;
}

void lista_operacoes()
{
    printf("\n  j - Jogar"
           "\n  a - Acrecentar ao banco de dados"
           "\n  v - Visualizar banco de dados"
           "\n  f - finalizar\n");
    printf("\nDigite o código da operação: ");
}

void imprimir_banco_de_dados()
{
    int i;
    char opc;
    dados d;

    arquivo_leitura = fopen("palavras.dat", "rb");
    if (arquivo_leitura == NULL)
    {
        printf("\n\tProblemas na abertura do arquivo\n\tNão existe o banco de dados");
        printf ("\n\nDeseja criar um banco de dados? [s/S = sim] [n/N = não]: ");
        fflush(stdin); scanf("%c",&opc);
        if(opc == 's' || opc == 'S')
            adicionar_ao_banco_de_dados();
        else
            exit(EXIT_FAILURE);
    }
    else
    {
        printf("\n\n       DICA      |"
               " QUANTIDADE DE PALAVRAS|"
               "                                PALAVRAS\n");
        for(i = 0; i < 114 ; i++)
            printf("-");
        printf("\n");
        while (!feof(arquivo_leitura))
        {
            fread(&d, sizeof(d), 1, arquivo_leitura);
            if(!feof(arquivo_leitura))
            {
                printf("%-17s| \t %-16d|", d.Pista, d.Qtd);
                for(i = 0; i < d.Qtd; i++)
                    printf("\t%-17s", d.Palavras[i]);
            }
            printf("\n");
        }
    }
    printf("Existem %d registros no banco de dados\n", ftell(arquivo_leitura)/sizeof(d));
    fclose(arquivo_leitura);
}

void adicionar_ao_banco_de_dados()
{
    int i;
    char opc;
    dados d;

    arquivo_leitura = fopen("palavras.dat", "ab");
    if (arquivo_leitura == NULL)
        printf("\n\tProblemas na abertura do arquivo\n\n");
    else
    {
        do
        {
            printf("\nDigite a dica (minímo 1 caracter e máximo 17 caracteres): ");
            fflush(stdin); scanf("%s",d.Pista);
            printf("\nDigite quantas palavras (minímo 1 e máximo 3): ");
            fflush(stdin); scanf("%d",&d.Qtd);
            if (d.Qtd > MAXPAL || d.Qtd < 1)
            {
                printf("\nQuantidade de palavras inválido ");
                printf ("\n\nDeseja recomeçar o cadastro? [s/S = sim] [n/N = não]:");
                fflush(stdin); scanf("%c",&opc);
                printf("\n");
            }
            else
            {
                for(i = 0; i < d.Qtd; i++)
                {
                    printf("\nDigite %dº palavra palavras (minímo 1 caracter e máximo 17 caracteres): ", i + 1);
                    fflush(stdin); scanf("%s",d.Palavras[i]);
                }
                fwrite(&d, sizeof(d), 1, arquivo_leitura);
                printf ("\nDeseja cadastrar outro? [s/S = sim] [n/N = não]: ");
                fflush(stdin); scanf("%c",&opc);
                printf("\n\n");
            }
        }while ( opc == 's' || opc == 'S');
    }
    fclose(arquivo_leitura);
}

void sortear_palavra(char Pi[], char Pa[][MAXCARC], int *k)
{
    int Qtd_registros, n, i;
    char opc;
    dados d;

    arquivo_leitura = fopen("palavras.dat", "rb");
    if (arquivo_leitura == NULL)
    {
        printf("\n\tProblemas na abertura do arquivo\n\tNão existe o banco de dados para iniciar o jogo");
        printf ("\n\nDeseja criar um banco de dados? [s/S = sim] [n/N = não]: ");
        fflush(stdin); scanf("%c",&opc);
        if(opc == 's' || opc == 'S')
            adicionar_ao_banco_de_dados();
        else
            exit(EXIT_FAILURE);
    }
    else
    {
        fseek(arquivo_leitura, 0, SEEK_END);
        Qtd_registros = ftell(arquivo_leitura)/sizeof(d);
        fseek(arquivo_leitura, 0, SEEK_SET);

        srand(time(NULL));
        fseek(arquivo_leitura, (rand() % Qtd_registros)*sizeof(d) , SEEK_CUR);

        fread(&d, sizeof(d), 1, arquivo_leitura);
        strcpy(Pi, d.Pista);
        *k = d.Qtd;
        for(i = 0; i < d.Qtd; i++)
            strcpy(Pa[i], d.Palavras[i]);
    }
    fclose(arquivo_leitura);
}

float sortear_valor_letra()
{
    float valor;
    int Qtd_registros;

    som_catraca();

    arquivo_leitura = fopen("premios.dat", "rb");
    if (arquivo_leitura == NULL)
    {
        printf("\n\tProblemas na abertura do arquivo\n\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fseek(arquivo_leitura, 0, SEEK_END);
        Qtd_registros = ftell(arquivo_leitura)/sizeof(valor);
        fseek(arquivo_leitura, 0, SEEK_SET);

        srand(time(NULL));
        fseek(arquivo_leitura, (rand() % Qtd_registros)*sizeof(valor) , SEEK_CUR);

        fread(&valor, sizeof(valor), 1, arquivo_leitura);

        fclose(arquivo_leitura);
    }
    return valor;
}

void exibir_tela(char Pi[], int n, char Pa[][MAXCARC], int comp[][MAXCARC])
{
    int i, k;

    printf("\n\tA palavra está associada com: \t%s\n", Pi);
    for (i = 0; i < n; i++)
    {
        printf("\n\t\t\t");
        for (k = 0; k < strlen(Pa[i]); k++)
        {
            if (comp[i][k] == 0)
                printf(" %c", Pa[i][k]);
            if (comp[i][k] == 1)
                printf(" _");
        }
        printf("\n");
    }
    printf("\n\n\t%-20s%-20s%-20s\n", j[0].nome, j[1].nome, j[2].nome);
    printf("\t===========\t    ===========\t\t===========\n");
    printf("\tR$ %5d,00\t    R$ %5d,00\t\tR$ %5d,00\n", j[0].valor, j[1].valor, j[2].valor);
}

int compara_palavras(char Ori[][MAXCARC], char Dig[][MAXCARC], int n)
{
    int retorno, i, k;
    retorno = 0;

    for (i = 0; i < n; i++)
        for (k = 0; k < strlen(Dig[i]); k++)
            Dig[i][k] = toupper(Dig[i][k]);

    for(i = 0; i < n ; i++)
        for(k = 0; k < strlen(Ori[i]) ; k++)
            if (strlen(Ori[i]) != strlen(Dig[i]) || Ori[i][k]!= Dig[i][k])
                retorno ++;

    return retorno;
}

int verifica_senha()
{
    char senha[7];
    int i;
    printf("\nDigite a senha para acesso ao banco de dados: ");
    for(i = 0; i < 7 ; i++)
    {
        fflush (stdin); senha[i] = getch(); printf ("*");
    }
    if (senha[0] == '8' && senha[1] == '9' && senha[2] == '4' && senha[3] == '3' && senha[4] == '5' && senha[5] == '8' && senha[6] == '1')
        return 1;
    else
        return 0;
}

void contagem_regressiva()
{
    int i;
    printf("\nVocê tem 5 segundos para pensar e depois digitar a palavra");
    for (i = 5; i >0; i--)
    {
        printf("...%d", i);
        sleep(1);
    }
    printf("\n");
}

void som_catraca()
{
    Beep(400, 600);
    Beep(350, 650);
    Beep(300, 700);
    Beep(250, 800);
    Beep(200, 900);
}

int letra_escolhida(char L, char letras[26])
{
    int cont, i;

    cont = 0;

    for(i = 0; i < strlen(letras); i++)
        if(letras[i] == L)
            cont++;
    if (cont == 0)
        letras[i] = L;
    else
        printf("\nLetra repetida, insira outra letra\n");

    return cont;
}
