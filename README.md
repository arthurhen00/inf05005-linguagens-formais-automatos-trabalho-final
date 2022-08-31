# formais-automatos-trabalho-final

![GIF](http://31.media.tumblr.com/27e4a2c5b7f04e0ab258259e20589778/tumblr_n57g6iyw2k1s4brmqo1_1280.gif)

# 1 Definição
Em grupos de 3 alunos, realizar as seguintes tarefas, divididas em 2 partes:

## 1. Cenário Real
(a) Selecionar um cenário de um sistema real que contenha, pelo menos, 5 operações/eventos relevantes;
(b) Definir formalmente a linguagem Lc que descreve o comportamento do sistema
escolhido, associando símbolos (alfabeto) às suas operações/eventos e apresentando as regras de formação de palavras (execuções válidas);
(c) Construir um autômato finito determinístico (AFD) Mc que reconheça a linguagem Lc;
(d) Criar uma lista de 10 palavras sobre o alfabeto de Mc, sendo que, destas, 5 devem
pertencer à ACEIT A(Mc) e 5, não;

## 2. Implementação
Criar um programa que, dado um AFD M que gera uma linguagem regular L, execute
as seguintes operações:
(a) Construa o autômato mínimo Mm equivalente a M usando o algoritmo visto em
aula;
(b) Dada uma lista de palavras, apresente todas as palavras w da lista tal que w ∈
ACEIT A(Mm);
(c) Determina se L é vazia usando Mm.

# 2 Instruções
• O programa do item 2 pode ser implementado usando a linguagem Java, C, C++ ou
Python, com código fonte bem documentado;
• O formato do arquivo de entrada contendo a definição do AFD deve seguir o seguinte
padrão:

M
S: q0,...,qN
A: a0,...,aN
i: qj
F: qf,...,qg
(q1,a,q2)
...
onde:
< M >: nome dado ao AFD;
S: conjunto dos estados de M com N ∈ N e N ≥ 0;
A: alfabeto de M com N ∈ N e N ≥ 0;
i: indica o estado qj ∈ S que é o estado inicial de M;
F: conjunto dos estados finais de M, para qf, ..., qg ∈ S;
(< qx >, < ay >, < qz >): transição da função programa de M, tal que q1, q2 ∈ S e
a ∈ A, indicando que, se a leitura do símbolo a no estado q1 leva M à transição para o
estado q2.

Exemplo:
MeuAFD
S: q0,q1,q2,q3
A: a,b
i: q0
F: q2,q3
(q0,a,q1)
(q0,b,q3)
(q1,a,q2)
(q1,b,q1)
(q2,a,q2)
(q3,b,q2)

• Todas as operações do programa devem seguir os algoritmos vistos em aula para garantir o resultado correto. Qualquer otimização ou alteração deve ser devidamente
documentada e associada a uma argumentação de correção;
• Todas as entradas (AFD e lista de palavras) devem ser fornecidas via seleção de arquivo, por teclado ou interface gráfica.

# 3 Documentação Necessária
1 Descrição em Português do cenário, das operações e da formalização da linguagem
(itens 1a e 1b);
2 Arquivo contendo o AFD do item 1c no formato de entrada definido.
3 Arquivo do JFLAP contendo o AFD do item 1c;
4 Arquivo com a lista de palavras do item 1d;
5 Fontes e executável do programa descrito no item 2 da Seção 1 (bem como qualquer
biblioteca/arquivo/instrução adicional necessário para execução do programa);
6 Breve tutorial de uso do programa desenvolvido.
Toda a documentação deve conter os nomes completos de todos os integrantes do
grupo.

# 4 Prazos
• Os itens 1 a 4 da Seção 3 devem ser entregues via Moodle na Etapa 1, conforme data
definida no Moodle da turma;
• Os itens 5 e 6 da Seção 3 devem ser entregues via Moodle na Etapa 2, conforme data
definida no Moodle da turma;

# 5 Avaliação
• Os trabalhos deverão ser apresentados de forma síncrona nas datas a serem divulgadas no Moodle, segundo ordem a ser informada;
• A apresentação consistirá de uma explanação sobre o cenário escolhido e sua representação como uma GLUD e da descrição e demonstração do programa desenvolvido.
Posteriormente à apresentação, os alunos do grupo serão questionados pelo professor;
• A avaliação do trabalho será composta da seguinte maneira: 50% pela avaliação da
documentação descrita na Seção 3 e 50% pela avaliação da apresentação pelo grupo;
• Os trabalhos cujos programas não seguirem os formatos de entradas e saídas descritos na Seção 2 serão desconsiderados;
• Documentação incorreta ou incompleta acarretará descontos na nota final;
• Documentos entregues com atraso implicam 20% de desconto, bem como a não apresentação na data marcada;
• A avaliação da apresentação será sobre a participação dos membros do grupo na explanação e o conhecimento sobre o trabalho, sendo que cada integrante do grupo poderá
ter uma avaliação individual, de acordo com sua participação e desenvoltura.