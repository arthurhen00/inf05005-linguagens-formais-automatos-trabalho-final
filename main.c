#include "functions.h"

int main(){

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

    minimizaAutomato(qntEstados, estados, estadoFinal, qntEstadosFinais, hash_map, qntSimbolos, alfabeto);

    processarListaPalavras("./entradas/entrada1.txt", hash_map, estadoInicial, estadoFinal, qntEstadosFinais);

    ehVazia(hash_map,estadoFinal,qntEstadosFinais,estadoInicial);

    //Imprime os estados e as transições dos mesmos
    /* for (int i = 0; i < qntEstados; i++)
    {
        printf("%s\n", estados[i]);
        imprime(hash_map[gerarHash(estados[i])]);
    } */

    return 0;
}