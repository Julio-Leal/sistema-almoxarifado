#ifndef MATERIAL_H
#define MATERIAL_H

/* Nó do estoque: cada material tem código, nome, quantidade e dois ramos da ABB. */
typedef struct NoMaterial {
    int codigo;
    char nome[100];
    int estoque;
    struct NoMaterial *esq;
    struct NoMaterial *dir;
} NoMaterial;

/* Pedido de um setor; prox aponta para o próximo pedido na fila. */
typedef struct Requisicao {
    int setor;
    int codigoMaterial;
    int quantidade;
    struct Requisicao *prox;
} Requisicao;

/* Fila FIFO: inicio é o próximo pedido e fim é o mais recente. */
typedef struct {
    Requisicao *inicio;
    Requisicao *fim;
} Fila;

/* Insere material na ABB (árvore binária de busca), mantendo códigos ordenados.
   Em código repetido, soma o estoque; falhas não alteram os dados existentes. */
NoMaterial *inserirMaterial(NoMaterial *raiz, int codigo,
                            const char nome[], int estoque);

/* Igual à inserção acima, mas retorna sucesso (1) ou falha (0). */
int inserirMaterialComResultado(NoMaterial **raiz, int codigo,
                                const char nome[], int estoque);

/* Busca pelo código e retorna o nó encontrado, ou NULL se não existir. */
NoMaterial *buscarMaterial(NoMaterial *raiz, int codigo);

/* Exibe todos os materiais em ordem crescente de código. */
void listarMateriais(NoMaterial *raiz);

/* Libera todos os nós da árvore. */
void liberarArvore(NoMaterial *raiz);

/* Inicializa uma fila vazia; os pedidos seguem FIFO (primeiro a entrar, primeiro a sair). */
void inicializarFila(Fila *fila);

/* Acrescenta um pedido ao fim da fila; uma quantidade não positiva é inválida. */
void enfileirar(Fila *fila, int setor, int codigoMaterial, int quantidade);

/* Retorna o primeiro pedido sem removê-lo, ou NULL se a fila estiver vazia. */
Requisicao *consultarInicio(Fila *fila);

/* Remove e libera o primeiro pedido da fila, se houver. */
void desenfileirar(Fila *fila);

/* Exibe os pedidos pendentes na ordem em que foram registrados. */
void listarRequisicoes(Fila *fila);

/* Remove e libera todos os pedidos pendentes. */
void liberarFila(Fila *fila);

/* Atende ou recusa os pedidos da fila e atualiza o estoque quando há saldo. */
void processarRequisicoes(NoMaterial *raiz, Fila *fila);

/* Insere os materiais de exemplo; retorna 0 se alguma inserção falhar. */
int carregarMateriaisIniciais(NoMaterial **raiz);

#endif
