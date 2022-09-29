#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 256
#define STRING_SIZE 64
#define SIMBOLO_VAZIO '$'
#define NUM_MAX_ESTADOS 32
#define NUM_MAX_ESTADOS_FINAIS 16

// Estrutura Lista
typedef struct tipoNo ptLSE;
struct tipoNo
{
    char simbolo[8];
    char estado[8];
    ptLSE *prox;
};

// Prototipos ** Listas **
ptLSE *criaLista();
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]);
ptLSE *remover(ptLSE *l, char estado[], int *terminou);
ptLSE *destroi(ptLSE *l);
int estaNaLista(ptLSE *lista, char estado[]);
int contaTamanhoLista(ptLSE *lista);

// Prototipos ** Hash Map **
int gerarHash(char estado[]);
void inicializarLista(ptLSE *hash_map[], int M);
void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[]);

// Prototipos *** Automato
void lerAutomato(char arq_leitura[], char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[], int *qntEstados, int *qntSimbolos, int *qntEstadosFinais);
void processarListaPalavras(char fileName[], ptLSE *map[], char estadoInicial[], char estadoFinal[][8], int qntEstadosFinais);

void remocaoEstadosInalcancaveis(ptLSE *map[], char estados[][8], char estadoFinal[][8], char *estadoInicial, int *qntEstados, int *qntEstadosFinais);
void achaEstadosAlcancaveis(ptLSE *estado, ptLSE *listaAlcancavel, ptLSE *map[]);
void atualizaLista(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaAlcancaveis, ptLSE *map[]);

void removeEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *map[]);
void achaEstadosInuteis(char estados[][8], char estadoFinal[][8], int *qntEstados, int *qntEstadosFinais, ptLSE *listaUteis, ptLSE *map[]);

void criaTotal(ptLSE *map[], int *qntEstados, char estados[][8], char alfabeto[][8], int qntSimbolos);

void minimizaAutomato(int qntEstados, char estados[][8], char estadoFinal[][8], int qntEstadosFinais, ptLSE *map[], int qntSimbolos, char alfabeto[][8]);
void dependencias(int p, int q, char estados[][8], ptLSE *map[], int qntSimbolos, char alfabeto[][8], int qntEstados, int matriz[][qntEstados - 1], int qntEstadosFinais, char estadoFinal[][8]);

int ehFinal(char estado[], char estadoFinal[][8], int qntEstadosFinais);
int comparaEstados(char estado1[], char estado2[], char estadoFinal[][8], int qntEstadosFinais);

void ehVazia(ptLSE *map[],char estadoFinal[][8],int qntEstadosFinais,char estadoInicial[]);

void juntaEstados(char estado1[], char estado2[], ptLSE *map[],char estados[][8], int qntEstados);

void gerarArquivoAutomatoMinimo(ptLSE *map[], char alfabeto[][8], int qntSimbolos, char afd_name[]);

// Prototipos *** Debug ***
void imprime(ptLSE *l);
void imprimeAutomato(ptLSE *map[], int qntEstados, char estados[][8]);