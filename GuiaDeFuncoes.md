# Guia rápido das funções C do projeto

Este guia resume as funções da biblioteca C e as principais funções do
almoxarifado. Exemplos de uso estão nos arquivos indicados ao final.

## Funções da biblioteca C

### Entrada e saída (`<stdio.h>`)

- **`printf`**: mostra texto formatado; `%d` representa um inteiro e `%s`, um
  texto.
- **`fprintf`**: como `printf`, mas permite escolher o destino. O projeto usa
  `stderr` para mensagens de erro.
- **`puts`**: mostra uma linha e acrescenta uma quebra de linha.
- **`fputs`**: mostra texto sem acrescentar quebra de linha.
- **`fgets`**: lê uma linha para um vetor, limitando o tamanho para evitar
  ultrapassar o espaço reservado. Retorna `NULL` no fim da entrada ou em caso
  de erro.
- **`getchar`**: lê um caractere; usado para descartar o restante de entradas
  longas.
- **`feof`**: verifica se uma leitura chegou ao fim da entrada.
- **`snprintf`**: copia texto formatado para um vetor sem ultrapassar seu
  tamanho.

`stdin`, `stdout` e `stderr` representam a entrada, a saída e a saída de erros.
`EOF` indica que não há mais dados para ler.

### Conversão, texto e memória

- **`strtol`** (`<stdlib.h>`): converte texto em número inteiro longo. O
  endereço `fim` indica onde a conversão parou, permitindo rejeitar entradas
  como `12abc`.
- **`malloc`** (`<stdlib.h>`): reserva memória dinâmica; pode falhar e retornar
  `NULL`.
- **`free`** (`<stdlib.h>`): libera memória antes reservada com `malloc`.
- **`strlen`** (`<string.h>`): conta os caracteres de um texto antes de `'\0'`.
- **`memmove`** (`<string.h>`): copia texto mesmo quando origem e destino se
  sobrepõem.
- **`isspace`** (`<ctype.h>`): identifica espaços, tabulações e quebras de
  linha.

### Limites e testes

- **`errno` e `ERANGE`**: permitem detectar números grandes demais durante a
  conversão com `strtol`.
- **`INT_MIN` e `INT_MAX`** (`<limits.h>`): menores e maiores valores de `int`;
  usados para evitar estouro.
- **`EXIT_FAILURE`** (`<stdlib.h>`): indica que o programa terminou com erro.
- **`assert`** (`<assert.h>`): verifica uma condição nos testes e sinaliza falha
  quando ela não é verdadeira.

## Funções do almoxarifado

Os protótipos ficam em [`include/material.h`](include/material.h) e
[`include/menu.h`](include/menu.h); as implementações estão em `main.c` e
`src/`.

### Árvore binária de busca (ABB)

A ABB guarda materiais por código: menores à esquerda e maiores à direita.

- **`inserirMaterial`**: insere um material e devolve a raiz da árvore.
- **`inserirMaterialComResultado`**: insere e informa sucesso (`1`) ou falha
  (`0`).
- **`inserirNo`**: auxiliar recursiva que encontra a posição do novo nó.
- **`buscarMaterial`**: procura um código; retorna `NULL` se não encontrar.
- **`listarMateriais`**: mostra os materiais em ordem crescente de código.
- **`liberarArvore`**: libera a memória de todos os nós.

### Fila de requisições

FIFO significa que o primeiro pedido registrado é o primeiro processado.

- **`inicializarFila`**: deixa a fila vazia.
- **`enfileirar`**: adiciona um pedido ao final.
- **`consultarInicio`**: consulta o primeiro pedido sem removê-lo.
- **`desenfileirar`**: remove e libera o primeiro pedido.
- **`listarRequisicoes`**: mostra os pedidos na ordem de chegada.
- **`liberarFila`**: remove e libera todos os pedidos restantes.

### Menu e operações

- **`executarMenu`**: apresenta as opções e chama a operação escolhida.
- **`exibirMenu`**: imprime as opções disponíveis.
- **`lerInteiro`**: lê e valida um número inteiro.
- **`lerQuantidade`**: repete a leitura até a quantidade atingir o mínimo.
- **`lerNome`**: lê o nome e rejeita texto vazio ou longo demais.
- **`cadastrarMaterial`**: cadastra material ou acrescenta unidades ao estoque.
- **`buscarEExibirMaterial`**: procura um material e mostra seus dados.
- **`registrarRequisicao`**: lê setor, material e quantidade e adiciona o pedido.
- **`carregarMateriaisIniciais`**: carrega o estoque de exemplo; retorna `0` se
  uma alocação falhar.
- **`processarRequisicoes`**: percorre a fila, atende ou recusa cada pedido,
  atualiza o estoque e mostra o resumo.
- **`main`**: inicializa os dados, chama o menu e libera a memória ao sair.

## Conceitos úteis

- **Ponteiro (`*`)** guarda o endereço de um dado; **`NULL`** indica que não
  aponta para nenhum objeto.
- **`&variavel`** passa o endereço da variável para uma função que precisa
  consultá-la ou alterá-la.
- **`struct`** agrupa dados relacionados, como código, nome e estoque de um
  material.
- **`sizeof`** informa o espaço em bytes necessário para um tipo ou objeto.
- **`static`** em uma função limita seu uso ao arquivo onde foi definida.
- **Recursão** ocorre quando uma função chama a si mesma para percorrer a ABB.
- Uma função **`void`** não retorna valor. `return` encerra uma função e pode
  devolver um resultado ao chamador.

Os testes também usam as funções auxiliares **`verificarOrdemCrescente`** e
**`contarNos`** para conferir a ABB. **`__wrap_malloc`** e **`__real_malloc`**
são símbolos específicos do linker GNU, usados para simular falhas de memória
nos testes; não fazem parte do programa normal nem da linguagem C padrão.

Para acompanhar o código, comece por [`main.c`](main.c), veja o menu em
[`src/menu.c`](src/menu.c), a ABB em [`src/arvore.c`](src/arvore.c), a fila em
[`src/fila.c`](src/fila.c) e o processamento em
[`src/processamento.c`](src/processamento.c).
