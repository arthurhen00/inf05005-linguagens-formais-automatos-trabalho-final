#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 256
#define STRING_SIZE 64
#define SIMBOLO_VAZIO '$'
#define NUM_MAX_ESTADOS 32
#define NUM_MAX_ESTADOS_FINAIS 16

typedef struct tipoNo ptLSE;
struct tipoNo
{
    char simbolo[8];
    char estado[8];
    ptLSE *prox;
};

void lerAutomato(char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[], int *qntEstados, int *qntSimbolos, int *qntEstadosFinais);
void processarListaPalavras(char fileName[], ptLSE *map[], char estadoInicial[], char estadoFinal[][8], int qntEstadosFinais);
void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[]);
int estaNaLista(ptLSE *lista, char estado[]);
ptLSE *remover(ptLSE *l, char estado[], int *terminou);
ptLSE *destroi(ptLSE *l);
void remocaoEstadosInalcancaveis(ptLSE *map[], char estados[][8], char estadoFinal[][8], char *estadoInicial, int *qntEstados, int *qntEstadosFinais);
void achaEstadosAlcancaveis(ptLSE *estado, ptLSE *listaAlcancavel, ptLSE *map[]);
void atualizaLista(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaAlcancaveis, ptLSE *map[]);
void removeEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *map[]);
void achaEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaUteis, ptLSE *map[]);
int contaTamanhoLista(ptLSE *lista);

void criaTotal(ptLSE *map[], int *qntEstados, char estados[][8], char alfabeto[][8], int qntSimbolos);

void minimizaAutomato(int qntEstados, char estados[][8], char estadoFinal[][8], int qntEstadosFinais, ptLSE *map[], int qntSimbolos, char alfabeto[][8]);
void dependencias(int p, int q, char estados[][8], ptLSE *map[], int qntSimbolos, char alfabeto[][8], int qntEstados, int matriz[][qntEstados - 1], int qntEstadosFinais, char estadoFinal[][8]);
int comparaEstados(char estado1[], char estado2[], char estadoFinal[][8], int qntEstadosFinais);
int ehFinal(char estado[], char estadoFinal[][8], int qntEstadosFinais);
void ehVazia(ptLSE *map[],char estadoFinal[][8],int qntEstadosFinais,char estadoInicial[]);

ptLSE *criaLista();
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]);
ptLSE *inserirInicio(ptLSE *ptLista, char simbolo[], char estado[]);
void inicializarLista(ptLSE *hash_map[], int M);
int gerarHash(char estado[]);
void juntaEstados(char estado1[], char estado2[], ptLSE *map[],char estados[][8], int qntEstados);

// DEBUG
void imprime(ptLSE *l);

int main()
{

    char automato_nome[STRING_SIZE];
    char estados[NUM_MAX_ESTADOS][8];
    char alfabeto[NUM_MAX_ESTADOS][8];
    char estadoInicial[8];
    char estadoFinal[NUM_MAX_ESTADOS_FINAIS][8];
    int qntEstados = 0, qntSimbolos = 0, qntEstadosFinais = 0;

    ptLSE *hash_map[101];
    inicializarLista(hash_map, 101);
    lerAutomato(automato_nome, estados, alfabeto, estadoInicial, estadoFinal, hash_map, &qntEstados, &qntSimbolos, &qntEstadosFinais);
    

    remocaoEstadosInalcancaveis(hash_map, estados, estadoFinal, estadoInicial, &qntEstados, &qntEstadosFinais);
    removeEstadosInuteis(estados, estadoFinal, &qntEstados, &qntEstadosFinais, hash_map);
    criaTotal(hash_map, &qntEstados, estados, alfabeto, qntSimbolos);

    //Imprime os estados e as transições dos mesmos
    /* for (int i = 0; i < qntEstados; i++)
    {
        printf("%s\n", estados[i]);
        imprime(hash_map[gerarHash(estados[i])]);
    } */

    minimizaAutomato(qntEstados, estados, estadoFinal, qntEstadosFinais, hash_map, qntSimbolos, alfabeto);

    processarListaPalavras("./entradas/entrada1.txt", hash_map, estadoInicial, estadoFinal, qntEstadosFinais);

    ehVazia(hash_map,estadoFinal,qntEstadosFinais,estadoInicial);
    return 0;
}

void ehVazia(ptLSE *map[],char estadoFinal[][8],int qntEstadosFinais,char estadoInicial[]){
    ptLSE *estadosAlcancaveis;
    int vazia=1;

    estadosAlcancaveis = criaLista();
    estadosAlcancaveis = inserirFim(estadosAlcancaveis, "\0", estadoInicial);
    achaEstadosAlcancaveis(map[gerarHash(estadoInicial)], estadosAlcancaveis, map);
    for(int i=0; i<qntEstadosFinais;i++){
        if(estaNaLista(estadosAlcancaveis, estadoFinal[i])){
            vazia=0;
        }
    }

    if(vazia){
        printf("Nao ha estados alcancaveis a partir do estado inicial");
    }else{
        printf("Ha estados alcancaveis a partir do estado inicial");
    }
}

void lerAutomato(char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[], int *qntEstados, int *qntSimbolos, int *qntEstadosFinais)
{
    char buffer[STRING_SIZE];
    char *token;

    FILE *arq = fopen("../automatos/teste.txt", "r");
    if (!arq)
    {
        printf("Erro ao abrir arquivo do automato --- (caminho nao encontrado)");
        fclose(arq);
        return;
    }

    fgets(name, STRING_SIZE, arq);
    name[strlen(name) - 1] = '\0';

    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++)
    {
        strcpy(estados[i], token);
        token = strtok(NULL, ",");
        (*qntEstados)++;
    }

    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++)
    {
        strcpy(alfabeto[i], token);
        token = strtok(NULL, ",");
        (*qntSimbolos)++;
    }

    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    strcpy(estadoInicial, token);

    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++)
    {
        strcpy(estadoFinal[i], token);
        token = strtok(NULL, ",");
        (*qntEstadosFinais)++;
    }

    while (!strcmp(fgets(buffer, STRING_SIZE, arq), "\n"))
    {
        // ignora os crlf
    }

    for (int i = 0; !feof(arq); i++)
    {
        char buffer_estado_fonte[8];
        char buffer_simbolo[8];
        char buffer_estado_destino[8];

        buffer[strlen(buffer) - 2] = '\0';

        token = strtok(buffer, "(");
        token = strtok(token, ",");
        strcpy(buffer_estado_fonte, token);

        token = strtok(NULL, ",");
        strcpy(buffer_simbolo, token);

        token = strtok(NULL, ",");
        strcpy(buffer_estado_destino, token);

        //printf("%s %s %s %d\n", buffer_estado_fonte, buffer_simbolo, buffer_estado_destino, i);

        int hash = gerarHash(buffer_estado_fonte);
        map[hash] = inserirFim(map[hash], buffer_simbolo, buffer_estado_destino);

        fgets(buffer, STRING_SIZE, arq);
    }

    fclose(arq);
}

void processarListaPalavras(char fileName[], ptLSE *map[], char estadoInicial[], char estadoFinal[][8], int qntEstadosFinais)
{

    FILE *arq = fopen(fileName, "r");
    if (!arq)
    {
        printf("Erro ao abrir arquivo de entrada --- (caminho nao encontrado)");
        fclose(arq);
        return;
    }

    char buffer_fita[STRING_SIZE];
    while (!feof(arq))
    {
        int aceita = 0;
        char buffer_estado[8];

        fscanf(arq, "%s", buffer_fita);
        strcpy(buffer_estado, estadoInicial);

        for (int i = 0; buffer_fita[i] != '\0'; i++)
        {

            int hash = gerarHash(buffer_estado);
            procurarMap(hash, map, buffer_fita[i], buffer_estado);
        }

        // MUDA PRA QUANDO TIVER MAIS DE 1 ESTADO FINAL

        // Verifica se parou em um estado final
        for (int i = 0; i < qntEstadosFinais; i++)
        {
            if (strcmp(buffer_estado, "-1") == 0 && strcmp(estadoInicial, estadoFinal[i]) == 0)
            {
                aceita = 1;
            }
            else if (strcmp(buffer_estado, estadoFinal[i]) == 0)
            {
                aceita = 1;
            }
        }

        if (aceita == 1)
        {
            printf("%32s | ACEITA\n", buffer_fita);
        }
        else
        {
            printf("%32s | REJEITA\n", buffer_fita);
        }
    }

    fclose(arq);
}

// LISTA -----------------------------------------------------------------------------------------------------------------------------------
// Cria lista
ptLSE *criaLista()
{
    return NULL;
}

// Insere na lista
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[])
{
    ptLSE *novo, *ptAux;

    novo = (ptLSE *)malloc(sizeof(ptLSE));
    strcpy(novo->simbolo, simbolo);
    strcpy(novo->estado, estado);

    if (ptLista == NULL)
    {
        novo->prox = ptLista;
        ptLista = novo;
    }
    else
    {
        for (ptAux = ptLista; ptAux->prox != NULL; ptAux = ptAux->prox)
        {
        }
        ptAux->prox = novo;
        novo->prox = NULL;
    }
    return ptLista;
}

int estaNaLista(ptLSE *lista, char estado[])
{
    ptLSE *ptAux;

    if (lista == NULL)
    {
        return 0;
    }
    else
    {
        if (strcmp(estado, lista->estado) == 0)
        {
            return 1;
        }
        else
        {
            return estaNaLista(lista->prox, estado);
        }
    }
}

// remove da lista os nodos com o estado passado
ptLSE *remover(ptLSE *l, char estado[], int *terminou)
{
    ptLSE *ant = NULL; // ponteiro auxiliar para a posi��o anterior
    ptLSE *ptaux = l;  // ponteiro auxiliar para percorrer a lista

    /*procura o elemento na lista*/
    while (ptaux != NULL && (strcmp(ptaux->estado, estado)))
    {
        ant = ptaux;
        ptaux = ptaux->prox;
    }

    /*verifica se achou*/
    if (ptaux == NULL)
    {
        *terminou = 1;
        return l; /*retorna a lista original*/
    }

    if (ant == NULL) /*vai remover o primeiro elemento*/
        l = ptaux->prox;
    else /*vai remover do meio ou do final*/
        ant->prox = ptaux->prox;

    free(ptaux); /*libera a mem�ria alocada*/

    return l;
}

ptLSE *destroi(ptLSE *l)
{
    ptLSE *ptaux; // ponteiro auxiliar para percorrer a lista
    while (l != NULL)
    {
        ptaux = l;
        l = l->prox;
        free(ptaux);
    }
    free(l);
    return NULL;
}

// Inicializa listas vazias no map
void inicializarLista(ptLSE *hash_map[], int M)
{
    for (int i = 0; i < M; i++)
    {
        ptLSE *ptLista = criaLista();
        hash_map[i] = ptLista;
    }
}

int gerarHash(char estado[])
{
    char buffer[strlen(estado)];
    strcpy(buffer, estado);
    for (int i = 0; i < 6; i++)
    {
        buffer[i] = buffer[i + 1];
    }
    return atoi(buffer);
}

void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[])
{

    ptLSE *ptAux;

    for (ptAux = map[hash]; ptAux != NULL; ptAux = ptAux->prox)
    {
        if (ptAux->simbolo[0] == simbolo)
        {
            strcpy(novoEstado, ptAux->estado);
            return;
        }
    }

    // excecao p/ palavra vazia
    if (simbolo == SIMBOLO_VAZIO)
    {
        strcpy(novoEstado, "-1");
        return;
    }

    // nao achou o simbolo
    strcpy(novoEstado, "-99");
}

// DEBUG
void imprime(ptLSE *l)
{
    ptLSE *ptaux;
    if (l == NULL)
        puts("lista vazia");
    else
        for (ptaux = l; ptaux != NULL; ptaux = ptaux->prox)
            printf("Estado(lista) = (%s,%s)\n", ptaux->simbolo, ptaux->estado);

    printf("\n");
}

void remocaoEstadosInalcancaveis(ptLSE *map[], char estados[][8], char estadoFinal[][8], char *estadoInicial, int *qntEstados, int *qntEstadosFinais)
{
    ptLSE *listaAlcancavel;

    listaAlcancavel = criaLista();

    // insere o estado inicial na lista (simbolo � irrelevante)
    listaAlcancavel = inserirFim(listaAlcancavel, "\0", estadoInicial);

    achaEstadosAlcancaveis(map[gerarHash(estadoInicial)], listaAlcancavel, map);

    
    atualizaLista(estados, estadoFinal, qntEstados, qntEstadosFinais, listaAlcancavel, map);
}

// percorre a lista de transi��o no map a partir do estado inicial e adiciona os estados alcan�aveis na lista
void achaEstadosAlcancaveis(ptLSE *estado, ptLSE *listaAlcancavel, ptLSE *map[])
{
    if (estado != NULL)
    {
        if (!estaNaLista(listaAlcancavel, estado->estado))
        {
            // insere o estado na lista de estados alcancaveis
            listaAlcancavel = inserirFim(listaAlcancavel, estado->simbolo, estado->estado);

            // chama a funcao na lista de transicao do prox estado
            int hash = gerarHash(estado->estado);
            achaEstadosAlcancaveis(map[hash], listaAlcancavel, map);

            if (estado->prox != NULL)
            {
                // chama a funcao para o prox elemento da lista
                achaEstadosAlcancaveis(estado->prox, listaAlcancavel, map);
            }
        }
        // chama a funcao para o prox elemento da lista
        achaEstadosAlcancaveis(estado->prox, listaAlcancavel, map);
    }
}

// recebe a lista de estados, a lista de estados alcan�aveis e o map, e remove os estados inalcancaveis;
void atualizaLista(char (*estados)[8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaAlcancaveis, ptLSE *map[])
{
    int i, j, k;

    // printf("***");
    // imprime(map[gerarHash(estados[1])]);

    for (i = 0; i < *qntEstados; i++)
    {
        if (!estaNaLista(listaAlcancaveis, estados[i]))
        {
            int eFinal = 0;
            int hash = gerarHash(estados[i]);

            // apaga a lista de transicao do estado no map
            map[hash] = destroi(map[hash]);

            // remove os nodos que referenciam o estado a ser excluido na lista de transi��o dos outros estados,
            // ACHO QUE VIAJEI E NAO PRECISA DISSO AQUI MAS FICA DE REFERENCIA PROS ESTADOS INUTEIS Q AI ISSO � UTIL (EU ACHO)
            for (j = 0; j < *qntEstados; j++)
            {
                int terminou = 0;
                int hash_percorre_map = gerarHash(estados[j]);
                while (!terminou)
                    map[hash_percorre_map] = remover(map[hash_percorre_map], estados[i], &terminou);
            }

            // procura e remove o estado da lista de estados finais
            for (j = 0; j < (*qntEstadosFinais); j++)
            {
                if (!strcmp(estadoFinal[j], estados[i]))
                {
                    for (k = j; k < (*qntEstadosFinais); k++)
                    {
                        if (k < NUM_MAX_ESTADOS_FINAIS)
                        {
                            strcpy(estadoFinal[k], estadoFinal[k + 1]);
                            eFinal = 1;
                        }
                    }
                }
            }

            // remove o estado da lista de estados
            for (j = i; j < *qntEstados; j++)
            {
                if (j < NUM_MAX_ESTADOS)
                {
                    strcpy(estados[j], estados[j + 1]);
                }
            }

            // atualiza os contadores
            (*qntEstados)--;
            if (eFinal)
                (*qntEstadosFinais)--;
        }
    }
}

void removeEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *map[])
{
    ptLSE *listaUteis = criaLista();
    int i, tamanhoIteracaoAnterior;

    // Insere os estados finais na lista
    for (i = 0; i < *qntEstadosFinais; i++)
    {
        listaUteis = inserirFim(listaUteis, "\0", estadoFinal[i]);
    }

    do
    {
        tamanhoIteracaoAnterior = contaTamanhoLista(listaUteis);
        achaEstadosInuteis(estados, estadoFinal, qntEstados, qntEstadosFinais, listaUteis, map);
    } while (tamanhoIteracaoAnterior != contaTamanhoLista(listaUteis));

    atualizaLista(estados, estadoFinal, qntEstados, qntEstadosFinais, listaUteis, map);
}

void achaEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaUteis, ptLSE *map[])
{
    int i = 0, hash;
    ptLSE *ptAux, *listaEstado;

    for (i = 0; i < *qntEstados; i++)
    {
        hash = gerarHash(estados[i]);
        // Acessa no map a lista do estado[i]
        ptAux = map[hash];
        // Percorre a lista do estado
        while (ptAux != NULL)
        {
            // Se achar algum estado que alcan�a um estado util adiciona o estado[i] na lista(se ele n�o estiver na lista)
            if (estaNaLista(listaUteis, ptAux->estado) && !estaNaLista(listaUteis, estados[i]))
                listaUteis = inserirFim(listaUteis, "\0", estados[i]);

            // Atualiza ponteiro que percorre a lista
            ptAux = ptAux->prox;
        }
    }
}

int contaTamanhoLista(ptLSE *lista)
{
    int tamanho = 0;
    ptLSE *ptAux = lista;

    while (ptAux != NULL)
    {
        tamanho++;
        ptAux = ptAux->prox;
    }

    return tamanho;
}

// -----------------
// func total
void criaTotal(ptLSE *map[], int *qntEstados, char estados[][8], char alfabeto[][8], int qntSimbolos)
{
    // dump definido por padrao como: q99
    int precisa_dump = 0;
    for (int i = 0; i < *qntEstados; i++)
    {

        ptLSE *ptAux = map[gerarHash(estados[i])];
        // printf("(%d) ", gerarHash(estados[i]));

        // estados com transicao
        for (int j = 0; j < qntSimbolos; j++)
        {
            int esta = 0;
            while (ptAux != NULL)
            {
                if (!strcmp(ptAux->simbolo, alfabeto[j]))
                {
                    // printf("[%s,%s] ", ptAux->simbolo, ptAux->estado);
                    esta = 1;
                } // printf("[%d - %s - %s]\n", esta, alfabeto[j], ptAux->estado);
                ptAux = ptAux->prox;
            }
            if (esta == 0)
            {
                map[gerarHash(estados[i])] = inserirFim(map[gerarHash(estados[i])], alfabeto[j], "q99");
                precisa_dump = 1;
            }
            ptAux = map[gerarHash(estados[i])];
        }

        // printf("\n");
    }

    // loop no dump
    if (precisa_dump == 1)
    {
        for (int i = 0; i < qntSimbolos; i++)
        {
            map[99] = inserirFim(map[99], alfabeto[i], "q99");
        }
        strcpy(estados[*qntEstados], "q99");
        (*qntEstados)++;
    }
}

//  Minimizacao
void minimizaAutomato(int qntEstados, char estados[][8], char estadoFinal[][8], int qntEstadosFinais, ptLSE *map[], int qntSimbolos, char alfabeto[][8])
{
    // -1 -> x
    // 0 -> dependencia
    // -1 n igual
    // 0 igual
    // n qtd de dependencias
    int tamanhoMatriz = qntEstados - 1;
    int matriz[tamanhoMatriz][tamanhoMatriz];

    for (int i = 0; i < tamanhoMatriz; i++)
    {
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            matriz[i][j] = 0;
        }
    }


    for (int j = 0; j < tamanhoMatriz; j++)
    {
        for (int i = tamanhoMatriz; i > j; i--)
        {
            //printf("[%s %s] %d %d\n", estados[i], estados[j], i, j);

            int resultado = comparaEstados(estados[i], estados[j], estadoFinal, qntEstadosFinais);
            if (resultado == -1)
            {
                matriz[i - 1][j] = -1;
            }
        }
        //printf("\n");
    }

    //printf("\n\n\n");
    int macete = 0;
    while (macete < 1000)
    {
        // laço para marcar dependencias
        for (int j = 0; j < tamanhoMatriz; j++)
        {
            for (int i = tamanhoMatriz; i > j; i--)
            {
                //printf("[[%s %s]] %d %d\n", estados[i], estados[j], i, j);

                int resultado = comparaEstados(estados[i], estados[j], estadoFinal, qntEstadosFinais);
                if (resultado == 0)
                {
                    dependencias(i, j, estados, map, qntSimbolos, alfabeto, qntEstados, matriz, qntEstadosFinais, estadoFinal);
                }
            }
            //printf("\n");
        }
        macete++;
    }



    for (int j = 0; j < tamanhoMatriz; j++)
    {
        for (int i = tamanhoMatriz; i > j; i--)
        {
            if(matriz[i-1][j] != -1 )
            {
                juntaEstados(estados[j],estados[i],map,estados,qntEstados);
            }
        }
    }
    

}
void juntaEstados(char estado1[], char estado2[], ptLSE *map[],char estados[][8], int qntEstados)
{
    ptLSE *ptAux1;

    map[gerarHash(estado2)]= NULL;

   for(int i = 0;i < qntEstados;i++){
        ptAux1=map[gerarHash(estados[i])];
        while(ptAux1 != NULL){
            if(!strcmp(ptAux1->estado,estado2)){
                strcpy(ptAux1->estado,estado1);
            }
            ptAux1=ptAux1->prox;
        }
   }

}



void dependencias(int p, int q, char estados[][8], ptLSE *map[], int qntSimbolos, char alfabeto[][8], int qntEstados, int matriz[][qntEstados - 1], int qntEstadosFinais, char estadoFinal[][8])
{
    ptLSE *ptAux1 = map[gerarHash(estados[p])];
    ptLSE *ptAux2 = map[gerarHash(estados[q])];

    int vet_aux[qntSimbolos];

    for (int i = 0; i < qntSimbolos; i++)
    {

        ptAux1 = map[gerarHash(estados[p])];
        ptAux2 = map[gerarHash(estados[q])];

        while (strcmp(ptAux1->simbolo, alfabeto[i]))
        {
            //printf("1.1- %s %s %s\n", ptAux1->simbolo, ptAux1->estado, alfabeto[i]);
            ptAux1 = ptAux1->prox;
            //printf("1.2- %s %s %s\n", ptAux1->simbolo, ptAux1->estado, alfabeto[i]);
        }
        while (strcmp(ptAux2->simbolo, alfabeto[i]))
        {
            //printf("2.1- %s %s %s\n", ptAux2->simbolo, ptAux2->estado, alfabeto[i]);
            ptAux2 = ptAux2->prox;
            //printf("2.2- %s %s %s\n", ptAux2->simbolo, ptAux2->estado, alfabeto[i]);
        }

        // printf("(%s %s %s)\n\n", ptAux1->estado, ptAux2->estado, alfabeto[i]);
        if (!strcmp(ptAux1->estado, ptAux2->estado))
        { // IGUAIS OU FINAIS
            // Estados Iguais
            // equivalentes
            vet_aux[i] = 1;
        }
        else
        {

            // Estados diferentes
            // olhar na matriz pra ver se esses estados tem X
            // se estiverem -> vet_aux = -1

            // forma os indice da matriz dado um par (estado1, estado2) -> indice matriz[a-1][b]

            // maior na esquerda (AQUI TA ZOANDO O MAP)
            char buffer1[8];
            char buffer2[8];
            strcpy(buffer1, ptAux1->estado);
            strcpy(buffer2, ptAux2->estado);
            if (gerarHash(ptAux1->estado) < gerarHash(ptAux2->estado))
            {
                char buffer[8];

                strcpy(buffer1, ptAux2->estado);
                strcpy(buffer2, ptAux1->estado);

                // strcpy(buffer, ptAux2->estado);
                // strcpy(ptAux2->estado, ptAux1->estado);
                // strcpy(ptAux1->estado, buffer);
            }
            //printf("((%s %s %s)\n\n", ptAux1->estado, ptAux2->estado, alfabeto[i]);

            int a, b;
            for (a = 0; a < qntEstados; a++)
            {
                if (!strcmp(estados[a], buffer1))
                    break;
            }
            for (b = 0; b < qntEstados; b++)
            {
                if (!strcmp(estados[b], buffer2))
                    break;
            }
            // ---------------------------------------------------------------------
            // estados[a] -> linha ; estados[b] -> coluna; a-1 coordenada da linhja; b coordenada da coluna
            //printf("#%s %s %d %d#\n", estados[a], estados[b], a, b);
            //printf("# %d #\n", matriz[a - 1][b]);

            // se o par (estado1, estado2) já esta marcado
            // [linha][coluna]
            if (matriz[a - 1][b] == -1)
            {
                vet_aux[i] = -1;
            }
            else
            {
                vet_aux[i] = 1;
            }

            // OS DOIS SAO FINAIS
            // for(int k = 0; k < qntEstadosFinais; k++){
            //    if(!strcmp(ptAux1->estado, estadoFinal[k])){
            //        for(int l = 0; l < qntEstadosFinais; l++){
            //            if(!strcmp(ptAux2->estado, estadoFinal[l])){
            //                vet_aux[i] = 1;
            //            }
            //        }
            //    }
            //}
        }
    }

    int n_eq = 1;
    for (int i = 0; i < qntSimbolos; i++)
    {
        if (vet_aux[i] == -1)
        {
            n_eq = 0;
        }
    }
    if (n_eq == 0)
    {
        //printf("\nnao eq\n");

        int a, b;
        for (a = 0; a < qntEstados; a++)
        {
            if (!strcmp(estados[a], estados[p]))
                break;
        }
        for (b = 0; b < qntEstados; b++)
        {
            if (!strcmp(estados[b], estados[q]))
                break;
        }
        //printf("*%s %s#\n", estados[p], estados[q]);
       // printf("*%s %s %d %d#\n", estados[a], estados[b], a, b);
       // printf("* %d #\n", matriz[a - 1][b]);
        matriz[a - 1][b] = -1;

        // return;
    }
    //printf("\n*************************************************\n");

    // faz o msm laco de antes, mas vendo o vet_aux, se tiver algum -1 nele pode marcar X direto
    // se nao tiver nada marcado com -1 no vet aux
    // ir recursivamente para o primeiro estado com dependencia e refazer essa funcao
}

int ehFinal(char estado[], char estadoFinal[][8], int qntEstadosFinais)
{
    for (int i = 0; i < qntEstadosFinais; i++)
    {
        if (!strcmp(estado, estadoFinal[i]))
        {
            return 1;
        }
    }
    return 0;
}

int comparaEstados(char estado1[], char estado2[], char estadoFinal[][8], int qntEstadosFinais)
{
    // 1 -> igual
    // 0 -> dependencia
    // -1 n igual
    if (ehFinal
(estado1, estadoFinal, qntEstadosFinais))
    {
        if (ehFinal
    (estado2, estadoFinal, qntEstadosFinais))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        if (ehFinal
    (estado2, estadoFinal, qntEstadosFinais))
            return -1;
        else
            return 0;
    }
}
