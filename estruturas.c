#include "functions.h"

// Listas ************************************************************************************************************************
// Inicializa a lista com NULL
ptLSE *criaLista(){
    return NULL;
}

// Insere no final da lista
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]){
    ptLSE *novo, *ptAux;

    novo = (ptLSE *)malloc(sizeof(ptLSE));
    strcpy(novo->simbolo, simbolo);
    strcpy(novo->estado, estado);

    if (ptLista == NULL){
        novo->prox = ptLista;
        ptLista = novo;
    }
    else{
        for (ptAux = ptLista; ptAux->prox != NULL; ptAux = ptAux->prox){

        }
        ptAux->prox = novo;
        novo->prox = NULL;
    }
    return ptLista;
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

// Apaga a lista
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

// Retorna 1 se o estado passado está na lista, 0 caso contrario
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

// Hash Map ************************************************************************************************************************
// Dado um estado no formato q<int> gera um hash para adicionalo no hash_map
// Padrao: hash = <int>
int gerarHash(char estado[]){
    char buffer[strlen(estado)];
    strcpy(buffer, estado);
    for (int i = 0; i < 6; i++){
        buffer[i] = buffer[i + 1];
    }
    return atoi(buffer);
}

// Inicializa listas vazias no map
void inicializarLista(ptLSE *hash_map[], int M){
    for (int i = 0; i < M; i++){
        ptLSE *ptLista = criaLista();
        hash_map[i] = ptLista;
    }
}

// Retorna o tamanho de uma lista
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
