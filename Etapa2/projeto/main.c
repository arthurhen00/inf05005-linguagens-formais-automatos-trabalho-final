#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define STRING_SIZE 256
#define SIMBOLO_VAZIO '$'
#define NUM_MAX_ESTADOS 32
#define NUM_MAX_ESTADOS_FINAIS 16

typedef struct tipoNo ptLSE;
struct tipoNo{
    char simbolo[8];
    char estado[8];
    ptLSE *prox;
};


void lerAutomato(char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[],int *qntEstados, int *qntSimbolos, int *qntEstadosFinais);
void processarListaPalavras(char fileName[STRING_SIZE], ptLSE *map[], char estadoInicial[], char estadoFinal[][8]);
void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[]);
int estaNaLista(ptLSE *lista,char estado[]);
ptLSE* remover(ptLSE* l, char estado[], int *terminou);
ptLSE* destroi(ptLSE* l);
void remocaoEstadosInalcancaveis(ptLSE *map[],char estados[][8], char estadoFinal[][8],char* estadoInicial,int *qntEstados, int *qntEstadosFinais);
void achaEstadosAlcancaveis(ptLSE *estado,ptLSE *listaAlcancavel,ptLSE *map[]);
void atualizaLista(char estados[][8],char estadoFinal[][8], int *qntEstados,int *qntEstadosFinais ,ptLSE *listaAlcancaveis, ptLSE *map[]);


ptLSE *criaLista();
ptLSE *inserirFim(ptLSE *ptLista, char simbolo[], char estado[]);
void inicializarLista(ptLSE *hash_map[], int M);
int gerarHash(char estado[]);

//DEBUG
void imprime(ptLSE* l);

int main(){

    char automato_nome[STRING_SIZE];
    char estados[NUM_MAX_ESTADOS][8];
    char alfabeto[NUM_MAX_ESTADOS][8];
    char estadoInicial[8];
    char estadoFinal[NUM_MAX_ESTADOS_FINAIS][8];
    int qntEstados = 0,qntSimbolos = 0, qntEstadosFinais = 0;

    ptLSE *hash_map[101];
    inicializarLista(hash_map, 101);
    lerAutomato(automato_nome, estados, alfabeto, estadoInicial, estadoFinal, hash_map,&qntEstados,&qntSimbolos,&qntEstadosFinais);
    printf("%s\n\n\n",estadoInicial);

    processarListaPalavras("entrada.txt", hash_map, estadoInicial, estadoFinal);

    remocaoEstadosInalcancaveis(hash_map,estados,estadoFinal,estadoInicial,&qntEstados,&qntEstadosFinais);

    return 0;
}

void lerAutomato(char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[],int *qntEstados, int *qntSimbolos, int *qntEstadosFinais){
    char buffer[STRING_SIZE];
    char *token;

    FILE *arq = fopen("../automatos/AFD _com_estado_inalcancavel.txt", "r");
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
            (*qntEstados)++;
        }

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        for(int i = 0; token != NULL; i++){
            strcpy(alfabeto[i], token);
            token = strtok(NULL, ",");
            (*qntSimbolos)++;
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
            (*qntEstadosFinais)++;
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

void processarListaPalavras(char fileName[STRING_SIZE], ptLSE *map[], char estadoInicial[], char estadoFinal[][8]){

    FILE *arq = fopen(fileName, "r");
        if(!arq){
            printf("Erro ao abrir arquivo de entrada --- (caminho nao encontrado)");
            fclose(arq);
            return;
        }

        char buffer_fita[STRING_SIZE];
        while(!feof(arq)){
            int aceita = 0;
            char buffer_estado[8];

            fscanf(arq, "%s", buffer_fita);
            strcpy(buffer_estado, estadoInicial);

            for(int i = 0; buffer_fita[i] != '\0'; i++){

                int hash = gerarHash(buffer_estado);
                procurarMap(hash, map, buffer_fita[i], buffer_estado);

            }

            // MUDA PRA QUANDO TIVER MAIS DE 1 ESTADO FINAL

            // Verifica se parou em um estado final
            for(int i = 0; i < 1; i++){
                if(strcmp(buffer_estado, "-1") == 0 && strcmp(estadoInicial, estadoFinal[i]) == 0){
                    aceita = 1;
                }else if(strcmp(buffer_estado, estadoFinal[i]) == 0){
                    aceita = 1;
                }
            }

            if(aceita == 1){
                printf("%32s | ACEITA\n", buffer_fita);
            } else {
                printf("%32s | REJEITA\n", buffer_fita);
            }

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

int estaNaLista(ptLSE *lista,char estado[]){
    ptLSE *ptAux;

    if(lista == NULL){
        return 0;
    }else{
        if(strcmp(estado,lista->estado) == 0){
            return 1;
        }else{
            return estaNaLista(lista->prox,estado);
        }
    }

}

//remove da lista os nodos com o estado passado
ptLSE* remover(ptLSE* l, char estado[], int *terminou)
{
     ptLSE *ant = NULL; //ponteiro auxiliar para a posição anterior
     ptLSE *ptaux = l; //ponteiro auxiliar para percorrer a lista

     /*procura o elemento na lista*/
     while (ptaux !=NULL && (strcmp(ptaux->estado, estado)))
     {
          ant = ptaux;
          ptaux = ptaux->prox;
     }

     /*verifica se achou*/
     if (ptaux == NULL){
        *terminou = 1;
        return l; /*retorna a lista original*/
     }


    if (ant == NULL) /*vai remover o primeiro elemento*/
      l = ptaux->prox;
    else /*vai remover do meio ou do final*/
      ant->prox = ptaux->prox;

    free(ptaux); /*libera a memória alocada*/

    return l;
}

ptLSE* destroi(ptLSE* l)
{
   ptLSE *ptaux; //ponteiro auxiliar para percorrer a lista
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
void inicializarLista(ptLSE *hash_map[], int M){
    for(int i = 0; i < M; i++){
        ptLSE *ptLista = criaLista();
        hash_map[i] = ptLista;
    }
}

int gerarHash(char estado[]){
    char buffer[strlen(estado)];
    strcpy(buffer,estado);
    for(int i = 0; i < 6; i++){
        buffer[i] = buffer[i+1];
    }
    return atoi(buffer);
}

void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[]){

    ptLSE *ptAux;

    for(ptAux = map[hash]; ptAux != NULL; ptAux = ptAux->prox){
        if(ptAux->simbolo[0] == simbolo){
            strcpy(novoEstado, ptAux->estado);
            return;
        }
    }

    //excecao p/ palavra vazia
    if(simbolo == SIMBOLO_VAZIO){
        strcpy(novoEstado, "-1");
        return;
    }

    // nao achou o simbolo
    strcpy(novoEstado, "-99");
}


//DEBUG
void imprime(ptLSE* l)
{
     ptLSE* ptaux;
     if (l == NULL)
        puts("lista vazia");
     else
     for (ptaux=l; ptaux!=NULL; ptaux=ptaux->prox)
         printf("Estado(lista) = %s\n",ptaux->estado);
}


void remocaoEstadosInalcancaveis(ptLSE *map[],char (*estados)[8], char estadoFinal[][8],char* estadoInicial,int *qntEstados, int *qntEstadosFinais){
    ptLSE *listaAlcancavel;

    listaAlcancavel = criaLista(listaAlcancavel);
    //insere o estado inicial na lista (simbolo é irrelevante)
    listaAlcancavel = inserirFim(listaAlcancavel,"aa",estadoInicial);

    achaEstadosAlcancaveis(map[gerarHash(estadoInicial)],listaAlcancavel,map);

    atualizaLista(estados,estadoFinal,qntEstados,qntEstadosFinais,listaAlcancavel,map);


}

//percorre a lista de transição no map a partir do estado inicial e adiciona os estados alcançaveis na lista
void achaEstadosAlcancaveis(ptLSE *estado,ptLSE *listaAlcancavel,ptLSE *map[]){
    if(estado != NULL){
        if(!estaNaLista(listaAlcancavel,estado->estado)){
            //insere o estado na lista de estados alcancaveis
            listaAlcancavel = inserirFim(listaAlcancavel,estado->simbolo,estado->estado);



            if(estado->prox != NULL){
                //chama a funcao para o prox elemento da lista
                achaEstadosAlcancaveis(estado->prox,listaAlcancavel,map);

                //chama a funcao na lista de transicao do prox estado
                int hash = gerarHash(estado->estado);
                achaEstadosAlcancaveis(map[hash],listaAlcancavel,map);
            }

        }
        //chama a funcao para o prox elemento da lista
        achaEstadosAlcancaveis(estado->prox,listaAlcancavel,map);
    }
}

//recebe a lista de estados, a lista de estados alcançaveis e o map, e remove os estados inalcancaveis;
void atualizaLista(char (*estados)[8],char estadoFinal[][8], int *qntEstados,int *qntEstadosFinais ,ptLSE *listaAlcancaveis, ptLSE *map[]){
    int i,j,k;

    for(i=0;i<*qntEstados;i++){
        if(!estaNaLista(listaAlcancaveis,estados[i])){
            int eFinal = 0;
            int hash = gerarHash(estados[i]);

            //apaga a lista de transicao do estado no map
            destroi(map[hash]);

            //remove os nodos que referenciam o estado a ser excluido na lista de transição dos outros estados,
            //ACHO QUE VIAJEI E NAO PRECISA DISSO AQUI MAS FICA DE REFERENCIA PROS ESTADOS INUTEIS Q AI ISSO É UTIL (EU ACHO)
            for(j = 0;j < *qntEstados;j++){
                int terminou = 0;
                int hash_percorre_map = gerarHash(estados[j]);
                ptLSE *ptEstado = map[hash_percorre_map];
                while(!terminou)
                    ptEstado = remover(ptEstado,estados[i],&terminou);
            }


            //procura e remove o estado da lista de estados finais
            for(j=0;j< (*qntEstadosFinais);j++){
                if(!strcmp(estadoFinal[j],estados[i])){
                    for(k = j ; k<(*qntEstadosFinais) ;k++){
                        if(k<NUM_MAX_ESTADOS_FINAIS){
                            strcpy(estadoFinal[k],estadoFinal[k+1]);
                            eFinal=1;
                        }
                    }
                }
            }


            //remove o estado da lista de estados
            for(j=i;j<*qntEstados;j++){
                    if(j<NUM_MAX_ESTADOS){
                        strcpy(estados[j],estados[j+1]);
                    }

            }

            //atualiza os contadores
            (*qntEstados)--;
            if(eFinal)
                (*qntEstadosFinais)--;


        }
    }

}
