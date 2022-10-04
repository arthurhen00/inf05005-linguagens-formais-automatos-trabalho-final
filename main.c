#include "functions.h"

int main(){

    char arq_leitura[STRING_SIZE];

    char automato_nome[STRING_SIZE];
    char estados[NUM_MAX_ESTADOS][8];
    char alfabeto[NUM_MAX_ESTADOS][8];
    char estadoInicial[8];
    char estadoFinal[NUM_MAX_ESTADOS_FINAIS][8];
    int qntEstados = 0, qntSimbolos = 0, qntEstadosFinais = 0;

    ptLSE *hash_map[101];
    inicializarLista(hash_map, 101);
    
    printf("\nInsira o nome do arquivo AFD: ");
    fgets(arq_leitura, STRING_SIZE, stdin);
    arq_leitura[strlen(arq_leitura) - 1] = '\0';
    lerAutomato(arq_leitura, automato_nome, estados, alfabeto, estadoInicial, estadoFinal, hash_map, &qntEstados, &qntSimbolos, &qntEstadosFinais);

    remocaoEstadosInalcancaveis(hash_map, estados, estadoFinal, estadoInicial, &qntEstados, &qntEstadosFinais);
    removeEstadosInuteis(estados, estadoFinal, &qntEstados, &qntEstadosFinais, hash_map);
    criaTotal(hash_map, &qntEstados, estados, alfabeto, qntSimbolos);

    minimizaAutomato(qntEstados, estados, estadoFinal, qntEstadosFinais, hash_map, qntSimbolos, alfabeto);

    removeEstadosInuteis(estados, estadoFinal, &qntEstados, &qntEstadosFinais, hash_map);

    printf("Insira o nome do arquivo Lista: ");
    fgets(arq_leitura, STRING_SIZE, stdin);
    arq_leitura[strlen(arq_leitura) - 1] = '\0';
    printf("\n");
    processarListaPalavras(arq_leitura, hash_map, estadoInicial, estadoFinal, qntEstadosFinais);
    printf("\n");
    ehVazia(hash_map,estadoFinal,qntEstadosFinais,estadoInicial);

    gerarArquivoAutomatoMinimo(hash_map,alfabeto, qntSimbolos, automato_nome, &qntEstados, &qntEstadosFinais, estados, estadoFinal, estadoInicial);

    //for(int i = 0; i < qntEstados; i++){
    //    printf("(%s)\n", estados[i]);
    //}
    //for(int i = 0; i < qntEstadosFinais; i++){
    //    printf("[%s]\n", estadoFinal[i]);
    //}imprimeAutomato(hash_map, qntEstados, estados);

    system("pause");

    return 0;
}

//