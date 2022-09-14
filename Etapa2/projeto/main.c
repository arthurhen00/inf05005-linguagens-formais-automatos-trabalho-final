#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define STRING_SIZE 256
#define SIMBOLO_VAZIO $

typedef struct tipoNo ptLSE;
struct tipoNo{
    char simbolo[8];
    char estado[8];
    ptLSE *prox;
};

void lerAutomato(char name[], char estados[][32], char alfabeto[][32], char estadoInicial[], char estadoFinal[][16], ptLSE *map[]);
void processarListaPalavras(char fileName[STRING_SIZE], ptLSE *map[]);

ptLSE *criaLista();
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]);
void inicializarLista(ptLSE *hash_map[], int M);
int gerarHash(char estado[]);

int main(){

    char automato_nome[STRING_SIZE];
    char estados[8][32];
    char alfabeto[8][32];
    char estadoInicial[8];
    char estadoFinal[8][16];

    ptLSE *hash_map[101];
    inicializarLista(hash_map, 101);
    lerAutomato(automato_nome, estados, alfabeto, estadoInicial, estadoFinal, hash_map);
    
    processarListaPalavras("entrada.txt", hash_map);

    return 0;
}

void lerAutomato(char name[], char estados[][32], char alfabeto[][32], char estadoInicial[], char estadoFinal[][16], ptLSE *map[]){
    char buffer[STRING_SIZE];
    char *token;

    FILE *arq = fopen("../automatos/AFD.txt", "r");
        if(!arq){
            printf("Erro ao abrir arquivo do automato --- (caminho nao encontrado)");
            fclose(arq);
            return;
        }

        fgets(name, STRING_SIZE, arq);
        name[strlen(name)-1] = '\0';

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        for(int i = 0; token != NULL; i++){
            strcpy(estados[i],token);
            token = strtok(NULL, ",");
        }

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        for(int i = 0; token != NULL; i++){
            strcpy(alfabeto[i], token);
            token = strtok(NULL, ",");
        }

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        strcpy(estadoInicial, token);

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        for(int i = 0; token != NULL; i++){
            strcpy(estadoFinal[i], token);
            token = strtok(NULL, ",");
        }

        while(!strcmp(fgets(buffer, STRING_SIZE, arq), "\n")){
            // ignora os crlf
        }

        for(int i = 0; !feof(arq); i++){
            char buffer_estado_fonte[8];
            char buffer_simbolo[8];
            char buffer_estado_destino[8];

            buffer[strlen(buffer)-2] = '\0';

            token = strtok(buffer, "(");
            token = strtok(token, ",");
            strcpy(buffer_estado_fonte, token);

            token = strtok(NULL, ",");
            strcpy(buffer_simbolo, token);

            token = strtok(NULL, ",");
            strcpy(buffer_estado_destino, token);

            int hash = gerarHash(buffer_estado_fonte);
            map[hash] = inserirFim(map[hash], buffer_simbolo, buffer_estado_destino);

            fgets(buffer, STRING_SIZE, arq);
        }

    fclose(arq);
}

void processarListaPalavras(char fileName[STRING_SIZE], ptLSE *map[]){

    FILE *arq = fopen(fileName, "r");
        if(!arq){
            printf("Erro ao abrir arquivo de entrada --- (caminho nao encontrado)");
            fclose(arq);
            return;
        }



    fclose(arq);

}

// LISTA -----------------------------------------------------------------------------------------------------------------------------------
// Cria lista
ptLSE *criaLista(){
    return NULL;
}

// Insere na lista
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]){
    ptLSE *novo, *ptAux;

    novo = (ptLSE*)malloc(sizeof(ptLSE));
    strcpy(novo->simbolo, simbolo);
    strcpy(novo->estado, estado);

    if(ptLista == NULL){
        novo->prox = ptLista;
        ptLista = novo;
    }else{
        for(ptAux = ptLista; ptAux->prox != NULL; ptAux = ptAux->prox){

        }
        ptAux->prox = novo;
        novo->prox = NULL;
    }
    return ptLista;
}

// Inicializa listas vazias no map
void inicializarLista(ptLSE *hash_map[], int M){
    for(int i = 0; i < M; i++){
        ptLSE *ptLista = criaLista();
        hash_map[i] = ptLista; 
    }
}

int gerarHash(char estado[]){
    for(int i = 0; i < 6; i++){
        estado[i] = estado[i+1];
    }
    return atoi(estado);
}
