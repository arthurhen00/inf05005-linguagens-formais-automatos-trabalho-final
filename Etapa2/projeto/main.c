#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define STRING_SIZE 256
#define SIMBOLO_VAZIO $

typedef struct tipoNo ptLSE;
struct tipoNo{
    char simbolo;
    char estado[3];
    ptLSE *prox;
};

void lerAutomato(char name[], char estados[][32], char alfabeto[][32], char estadoInicial[], char estadoFinal[][16], ptLSE *map[]);

ptLSE *inserirFim(ptLSE *ptLista, char simbolo, char estado[]);

int main(){

    char automato_nome[STRING_SIZE];
    char estados[3][32];
    char alfabeto[1][32];
    char estadoInicial[3];
    char estadoFinal[3][16];

    ptLSE *hash_map[101];
    lerAutomato(automato_nome, estados, alfabeto, estadoInicial, estadoFinal, hash_map);




    return 0;
}

void lerAutomato(char name[], char estados[][32], char alfabeto[][32], char estadoInicial[], char estadoFinal[][16], ptLSE *map[]){
    char buffer[STRING_SIZE];
    char *token;

    FILE *arq = fopen("../automatos/AFD.txt", "r");
        if(!arq){
            printf("Erro ao abrir arquivo --- (caminho nao encontrado)");
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
            char buffer_estado_fonte[3];
            char buffet_simbolo;
            char buffer_estado_destino[3];

            buffer[strlen(buffer)-1] = '\0';
            printf("%s\n", buffer);
            token = strtok(buffer, "(");
            printf("%s\n", token);
            token = strtok(token, ",");
            printf("%s\n", token);
            token = strtok(NULL, ",");
            printf("%s\n", token);
            token = strtok(NULL, ",");
            token[strlen(token)-1] = '\0';
            printf("%s\n", token);

            /*
            Inserir no map
            */

            fgets(buffer, STRING_SIZE, arq);
        }

    fclose(arq);
}



// LISTA -----------------------------------------------------------------------------------------------------------------------------------
// Cria lista
ptLSE *criaLista(){
    return NULL;
}

// Insere na lista
ptLSE *inserirFim(ptLSE *ptLista, char simbolo, char estado[]){
    ptLSE *novo, *ptAux;

    novo = (ptLSE*)malloc(sizeof(ptLSE));
    novo->simbolo = simbolo;
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
