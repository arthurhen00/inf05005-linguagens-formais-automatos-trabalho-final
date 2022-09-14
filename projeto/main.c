#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME_SIZE 64
#define STRING_SIZE 256

void lerAutomato();

int main(){

    lerAutomato();

    return 0;
}

void lerAutomato(){

    char buffer[STRING_SIZE];
    char *token;

    char automato_nome[STRING_SIZE];
    int estadoInicial;
    int estadoFinal[16];

    FILE *arq = fopen("../automatos/AFD.txt", "r");
        if(!arq){
            printf("Erro ao abrir arquivo --- (caminho nao encontrado)");
            fclose(arq);
            return;
        }
        
        fgets(automato_nome, STRING_SIZE, arq);
        automato_nome[strlen(automato_nome)-1] = '\0';

        fgets(buffer, STRING_SIZE, arq);
        buffer[strlen(buffer)-1] = '\0';
        printf("%s", buffer);

        token = strtok(buffer, ":");
        token = strtok(NULL, ",");
        while(token != NULL){
            printf("\n%s", token);
            token = strtok(NULL, ",");
        }
        

    fclose(arq);

}