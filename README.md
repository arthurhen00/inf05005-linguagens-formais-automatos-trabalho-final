## formais-automatos-trabalho-final

## Comandos para compilar:

`gcc -c main.c estruturas.c automatos.c`  
`gcc -o exe main.o estruturas.o automatos.o`  
`./exe`  

### Arquivo de entrada:  
Arquivos contendo automatos devem ser colocados na pasta `automatos`.  
Arquivos contendo lista de palavras para processamento devem ser colocados na pasta `entradas`.  
O simbolo '$' representa palavra vazia.  
O estado q99 é para uso exclusivo do programa.  
A nomeação dos estados deve ser do tipo q<número>, onde <número> é um valor entre 0 e 98.  
A quantidade máxima de estados deve ser 31.  
Não deve conter espaços.  
Após a última transição deve haver um CRLF. É importante ter uma última linha vazia no arquivo.  
Ex:  
![IMG](https://media.discordapp.net/attachments/1024803269043830986/1024803541300297728/unknown.png?width=350&height=500)

### Arquivo de saída:  
O arquivo de saída será no formato `<nome afd>_minimo.txt`.  

### Funcionamento:  
Input:  
Nome do arquivo com o autômato.  
Nome do arquivo com as listas de palavras.  

Output:  
Arquivo de saída com o afd minimizado.  
Conjunto de ACEITA e REJEITA no terminal.  
AFD recebe L vazia? no terminal.  
