#include "functions.h"

// Dado o nome de um arquivo que contem um AFD, faz a leitura dele
// e o armazena em um hash map
void lerAutomato(char arq_leitura[], char name[], char estados[][8], char alfabeto[][8], char estadoInicial[], char estadoFinal[][8], ptLSE *map[], int *qntEstados, int *qntSimbolos, int *qntEstadosFinais){
    char buffer[STRING_SIZE];
    char *token;
    char dir[STRING_SIZE] = "./automatos/"; 

    strcat(dir, arq_leitura);
    FILE *arq = fopen(dir, "r");
    if (!arq){
        printf("Erro ao abrir arquivo do automato --- (caminho nao encontrado '%s')\n", dir);
        fclose(arq);
        exit(3);
        return;
    }
    // Nome do automato
    fgets(name, STRING_SIZE, arq);
    name[strlen(name) - 1] = '\0';
    // Leitura dos estados
    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++){
        strcpy(estados[i], token);
        token = strtok(NULL, ",");
        (*qntEstados)++;
    }
    // Leitura do alfabeto
    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++){
        strcpy(alfabeto[i], token);
        token = strtok(NULL, ",");
        (*qntSimbolos)++;
    }
    // Leitura do estado inicial
    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    strcpy(estadoInicial, token);
    // Leitura dos estados finais
    fgets(buffer, STRING_SIZE, arq);
    buffer[strlen(buffer) - 1] = '\0';
    token = strtok(buffer, ":");
    token = strtok(NULL, ",");
    for (int i = 0; token != NULL; i++){
        strcpy(estadoFinal[i], token);
        token = strtok(NULL, ",");
        (*qntEstadosFinais)++;
    }

    while (!strcmp(fgets(buffer, STRING_SIZE, arq), "\n")){
        // ignora os crlf
    }
    // Leitura das transicoes
    for (int i = 0; !feof(arq); i++){
        char buffer_estado_fonte[8];
        char buffer_simbolo[8];
        char buffer_estado_destino[8];

        if(buffer[strlen(buffer) - 1] == '\n'){
            buffer[strlen(buffer) - 2] = '\0';
        }else{
            buffer[strlen(buffer) - 1] = '\0';
        }

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

// dado um arquivo com palavras, imprime aceita caso o automato no hash_map o aceita, rejeita caso contrario
void processarListaPalavras(char fileName[], ptLSE *map[], char estadoInicial[], char estadoFinal[][8], int qntEstadosFinais){

    char dir[STRING_SIZE] = "./entradas/";

    strcat(dir, fileName);
    FILE *arq = fopen(dir, "r");
    if (!arq){
        printf("Erro ao abrir arquivo de entrada --- (caminho nao encontrado '%s')\n", dir);
        fclose(arq);
        exit(3);
        return;
    }

    char buffer_fita[STRING_SIZE];
    while (!feof(arq)){
        int aceita = 0;
        char buffer_estado[8];

        fscanf(arq, "%s", buffer_fita);
        strcpy(buffer_estado, estadoInicial);

        for (int i = 0; buffer_fita[i] != '\0'; i++){

            int hash = gerarHash(buffer_estado);
            procurarMap(hash, map, buffer_fita[i], buffer_estado);
        }

        // Verifica se parou em um estado final
        for (int i = 0; i < qntEstadosFinais; i++){
            if (strcmp(buffer_estado, "-1") == 0 && strcmp(estadoInicial, estadoFinal[i]) == 0){
                aceita = 1;}
            else if (strcmp(buffer_estado, estadoFinal[i]) == 0){
                aceita = 1;
            }
        }

        if (aceita == 1){
            printf("%32s | ACEITA\n", buffer_fita);
        }else{
            printf("%32s | REJEITA\n", buffer_fita);
        }
    }

    fclose(arq);
}

// Dado um hash e um map, procura um simbolo nesse map
// Se achar retorna no "novoEstado" o estado ele aponta
void procurarMap(int hash, ptLSE *map[], char simbolo, char novoEstado[]){

    ptLSE *ptAux;

    for (ptAux = map[hash]; ptAux != NULL; ptAux = ptAux->prox){
        if (ptAux->simbolo[0] == simbolo){
            strcpy(novoEstado, ptAux->estado);
            return;
        }
    }

    // excecao p/ palavra vazia
    if (simbolo == SIMBOLO_VAZIO){
        strcpy(novoEstado, "-1");
        return;
    }

    // nao achou o simbolo
    strcpy(novoEstado, "-99");
}

// Dado um AFD armazenado em um hash map, remove os estados inalcancaveis desse AFD do map
void remocaoEstadosInalcancaveis(ptLSE *map[], char estados[][8], char estadoFinal[][8], char *estadoInicial, int *qntEstados, int *qntEstadosFinais)
{
    ptLSE *listaAlcancavel;
    listaAlcancavel = criaLista();

    // insere o estado inicial na lista (simbolo � irrelevante)
    listaAlcancavel = inserirFim(listaAlcancavel, "\0", estadoInicial);

    achaEstadosAlcancaveis(map[gerarHash(estadoInicial)], listaAlcancavel, map);

    atualizaLista(estados, estadoFinal, qntEstados, qntEstadosFinais, listaAlcancavel, map);
}

// percorre a lista de transicao no map a partir do estado inicial e adiciona os estados alcancaveis na lista
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

// recebe a lista de estados, a lista de estados alcancaveis e o map, e remove os estados inalcancaveis;
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

// remove os estados que nao estao na lista de estados uteis
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

// cria uma lista com os estados uteis
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
            // Se achar algum estado que alcanca um estado util adiciona o estado[i] na lista(se ele nao estiver na lista)
            if (estaNaLista(listaUteis, ptAux->estado) && !estaNaLista(listaUteis, estados[i]))
                listaUteis = inserirFim(listaUteis, "\0", estados[i]);

            // Atualiza ponteiro que percorre a lista
            ptAux = ptAux->prox;
        }
    }
}

// procura o simbolo [j] em todos os hashs, caso o simbolo nao esteja, adiciona
void criaTotal(ptLSE *map[], int *qntEstados, char estados[][8], char alfabeto[][8], int qntSimbolos){
    // dump definido por padrao como: q99
    int precisa_dump = 0;
    for (int i = 0; i < *qntEstados; i++){

        ptLSE *ptAux = map[gerarHash(estados[i])];

        for (int j = 0; j < qntSimbolos; j++){
            int esta = 0;
            // procura o simbolo [j] em todos os hashs
            while (ptAux != NULL){
                if (!strcmp(ptAux->simbolo, alfabeto[j])){
                    esta = 1;
                }
                ptAux = ptAux->prox;
            }
            // caso o simbolo nao esteja, adiciona
            if (esta == 0){
                map[gerarHash(estados[i])] = inserirFim(map[gerarHash(estados[i])], alfabeto[j], "q99");
                precisa_dump = 1;
            }
            ptAux = map[gerarHash(estados[i])];
        }
    }

    // loop no dump
    if (precisa_dump == 1){
        for (int i = 0; i < qntSimbolos; i++){
            map[99] = inserirFim(map[99], alfabeto[i], "q99");
        }
        strcpy(estados[*qntEstados], "q99");
        (*qntEstados)++;
    }
}

//  Minimizacao
void minimizaAutomato(int qntEstados, char estados[][8], char estadoFinal[][8], int qntEstadosFinais, ptLSE *map[], int qntSimbolos, char alfabeto[][8]){
    // -1 -> x
    // 0 -> dependencia

    int tamanhoMatriz = qntEstados - 1;
    int matriz[tamanhoMatriz][tamanhoMatriz];

    // Cria a matriz de dependencia
    for (int i = 0; i < tamanhoMatriz; i++){
        for (int j = 0; j < tamanhoMatriz; j++){
            matriz[i][j] = 0;
        }
    }

    // Verifica na matriz, os pares que sao do tipo (Final, N_Final), (N_Final, Final), e já marca eles
    // como nao equivalentes (-1)
    for (int j = 0; j < tamanhoMatriz; j++){
        for (int i = tamanhoMatriz; i > j; i--){
            int resultado = comparaEstados(estados[i], estados[j], estadoFinal, qntEstadosFinais);
            if (resultado == -1){
                matriz[i - 1][j] = -1;
            }
        }
    }

    // Rodar o programa x vezes até nao ter mais dependencias
    // daria pra mudar usando uma flag, quando eu nao alterar nada na matriz, while = 0
    int macete = 0;
    while (macete < 1000){
        // laço para marcar dependencias
        for (int j = 0; j < tamanhoMatriz; j++){
            for (int i = tamanhoMatriz; i > j; i--){

                int resultado = comparaEstados(estados[i], estados[j], estadoFinal, qntEstadosFinais);
                // Caso tenha um espaco em branco na matriz (0), olhar para as dependencias
                if (resultado == 0){
                    dependencias(i, j, estados, map, qntSimbolos, alfabeto, qntEstados, matriz, qntEstadosFinais, estadoFinal);
                }
            }
        }
        macete++;
    }

    // junta os estados (estado1, estado2) da matriz
    for (int j = 0; j < tamanhoMatriz; j++){
        for (int i = tamanhoMatriz; i > j; i--){
            if(matriz[i-1][j] != -1 ){
                juntaEstados(estados[j],estados[i],map,estados,qntEstados);
            }
        }
    }

    // DEBUG
    //for (int j = tamanhoMatriz - 1; j >= 0; j--)
    //{
    //    for (int i = 0; i < j+1; i++)
    //    {
    //        printf("%3d|", matriz[j][i]);
    //    }
    //    printf("\n");
 
    //}


}

// dado um par (estado1, estado2) olha na matriz se esse par é marcado com X, caso for, marca com X o estado que depende desse,
// caso contrario, nao faz nada
void dependencias(int p, int q, char estados[][8], ptLSE *map[], int qntSimbolos, char alfabeto[][8], int qntEstados, int matriz[][qntEstados - 1], int qntEstadosFinais, char estadoFinal[][8])
{
    ptLSE *ptAux1 = map[gerarHash(estados[p])];
    ptLSE *ptAux2 = map[gerarHash(estados[q])];

    // Lógica do vetor auxiliar,
    // faz todos os caminhamentos de um estado
    // se algum desses pares já esta marcado (-1) na matriz
    // marca uma posicao desse vetor -1
    // no final analisa o vetor, se encontra algum -1
    // marca X (-1) no estado que dependia desse par de estado
    // *** Dava só pra ter usado uma flag ***
    int vet_aux[qntSimbolos];

    for (int i = 0; i < qntSimbolos; i++){

        ptAux1 = map[gerarHash(estados[p])];
        ptAux2 = map[gerarHash(estados[q])];

        // estados que transitam com o mesmo simbolo
        while (strcmp(ptAux1->simbolo, alfabeto[i])){
            ptAux1 = ptAux1->prox;
        }
        while (strcmp(ptAux2->simbolo, alfabeto[i])){
            ptAux2 = ptAux2->prox;
        }

        // Estados iguais sao equivalentes
        if (!strcmp(ptAux1->estado, ptAux2->estado)){
            vet_aux[i] = 1;
        }else{ // Nao iguais, podem nao ser equivalentes

            // Estados diferentes
            // olhar na matriz pra ver se esses estados tem X
            // se estiverem -> vet_aux = -1
            // forma os indice da matriz dado um par (estado1, estado2) -> indice matriz[a-1][b]

            // Estado com o maior Indice SEMPRE deve fica na Esquerda (Interfere na matriz)
            char buffer1[8];
            char buffer2[8];
            strcpy(buffer1, ptAux1->estado);
            strcpy(buffer2, ptAux2->estado);
            if (gerarHash(ptAux1->estado) < gerarHash(ptAux2->estado)){
                strcpy(buffer1, ptAux2->estado);
                strcpy(buffer2, ptAux1->estado);
            }

            // Encontra os indices de 2 estados na matriz
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

            // estados[a] -> linha ; estados[b] -> coluna; a-1 coordenada da linhja; b coordenada da coluna
            // se o par (estado1, estado2) já esta marcado
            // [linha][coluna]
            if (matriz[a - 1][b] == -1){
                vet_aux[i] = -1;
            }else{
                vet_aux[i] = 1;
            }
        }
    }

    // Verifica se algum par de estados nao equivalentes caiu no vetor auxiliar
    int n_eq = 1;
    for (int i = 0; i < qntSimbolos; i++){
        if (vet_aux[i] == -1){
            n_eq = 0;
        }
    }

    // Se tiver vetores nao equivalentes, quer dizer que alguma dependencia do par passado
    // já era marcado na matriz, logo devo marcar o par passado na matriz
    if (n_eq == 0){
        // Encontra os indices de 2 estados na matriz
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

        // Atualiza a matriz
        matriz[a - 1][b] = -1;
    }
}

// Compara 2 estados, se for do tipo (Final, N_Final) || (N_Final, Final) retorna 1, 0 caso contrario
int comparaEstados(char estado1[], char estado2[], char estadoFinal[][8], int qntEstadosFinais){
    if (ehFinal(estado1, estadoFinal, qntEstadosFinais)){
        if (ehFinal(estado2, estadoFinal, qntEstadosFinais)){
            return 0;
        }else{
            return -1;
        }
    }else{
        if (ehFinal(estado2, estadoFinal, qntEstadosFinais))
            return -1;
        else
            return 0;
    }
}

// Retorna 1 se o estado passado for final
int ehFinal(char estado[], char estadoFinal[][8], int qntEstadosFinais){
    for (int i = 0; i < qntEstadosFinais; i++){
        if (!strcmp(estado, estadoFinal[i])){
            return 1;
        }
    }
    return 0;
}

// dados 2 estados equivalentes, atualiza o map juntando eles
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

// dado um map que armazena um AFD, informa se L gerada pelo AFD é uma linguagem vazia
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
        printf("Nao ha estados alcancaveis a partir do estado inicial (L eh vazia)\n\n");
    }else{
        printf("Ha estados alcancaveis a partir do estado inicial (L nao eh vazia)\n\n");
    }
}

// Gera um arquivo para o automato minimo
void gerarArquivoAutomatoMinimo(ptLSE *map[], char alfabeto[][8], int qntSimbolos, char afd_name[], int *qntEstados, int *qntEstadoFinal, char estados[][8], char estadoFinal[][8], char estadoInicial[]){
    char dir[STRING_SIZE] = "./automatos/";
    char estados_remov[NUM_MAX_ESTADOS][8];
    int qnt_remov = 0;

    // coloca os estados que serao removidos em um vet aux
    for(int i = 0; i < *qntEstados; i++){
        if(map[gerarHash(estados[i])] == NULL){
            strcpy(estados_remov[qnt_remov], estados[i]);
            qnt_remov++;
        }
    }

    // atualiza estadosFinais
    char buffer0[NUM_MAX_ESTADOS][8];
    int count0 = 0;
    for(int i = 0; i < *qntEstadoFinal; i++){
        int flag = 0;
        for(int j = 0; j < qnt_remov; j++){
            if(!strcmp(estadoFinal[i], estados_remov[j])){
                flag = 1;
            }
        }
        if(flag == 0){
            strcpy(buffer0[count0], estadoFinal[i]);
            count0++;
        }
    }
    for(int i = 0; i < count0; i++){
        strcpy(estadoFinal[i], buffer0[i]);
    }
    (*qntEstadoFinal) = count0;

    // atualiza estados
    char buffer[NUM_MAX_ESTADOS][8];
    int count = 0;
    for(int i = 0; i < *qntEstados; i++){
        int flag = 0;
        for(int j = 0; j < qnt_remov; j++){
            if(!strcmp(estados[i], estados_remov[j])){
                flag = 1;
            }
        }
        if(flag == 0){
            strcpy(buffer[count], estados[i]);
            count++;
        }
    }
    for(int i = 0; i < count; i++){
        strcpy(estados[i], buffer[i]);
    }
    (*qntEstados) = count;

    char name[STRING_SIZE];
    strcpy(name, afd_name);
    strcat(afd_name, "_minimo.txt");
    strcat(dir, afd_name);
    FILE *arq = fopen(dir, "w");
    
    fprintf(arq, "%s_minimo\n", name);
    fprintf(arq, "%s", "S:");
    for(int i = 0; i < *qntEstados; i++){
        if(i < *qntEstados - 1){
            fprintf(arq, "%s,", estados[i]);
        }else{
            fprintf(arq, "%s\n", estados[i]);
        }
    }
    fprintf(arq, "%s", "A:");
    for(int i = 0; i < qntSimbolos; i ++){
        if(i < qntSimbolos - 1){
            fprintf(arq, "%s,", alfabeto[i]);
        }else{
            fprintf(arq, "%s\n", alfabeto[i]);
        }
    }
    fprintf(arq, "%s%s\n", "i:", estadoInicial);
    fprintf(arq, "%s", "F:");
    for(int i = 0; i < *qntEstadoFinal; i ++){
        if(i < *qntEstadoFinal - 1){
            fprintf(arq, "%s,", estadoFinal[i]);
        }else{
            fprintf(arq, "%s\n", estadoFinal[i]);
        }
    }
    fprintf(arq, "%s", "\n");

    for (int i = 0; i < *qntEstados; i++){
        ptLSE *ptAux = map[gerarHash(estados[i])];
        if (ptAux != NULL){
            for (ptAux; ptAux != NULL; ptAux = ptAux->prox){
                fprintf(arq, "(%s,", estados[i]);
                fprintf(arq, "%s,", ptAux->simbolo);
                fprintf(arq, "%s)", ptAux->estado);
                fprintf(arq, "%s", "\n");
            }
        }
    }
    

    fclose(arq);

}
    