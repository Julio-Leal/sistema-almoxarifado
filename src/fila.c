#include <stdio.h>
#include <stdlib.h>

#include "material.h"

/* Marca a fila como vazia, sem pedidos no início nem no fim. */
void inicializarFila(Fila *fila) {
    if (fila == NULL) {
        fprintf(stderr, "Erro: fila invalida.\n");
        return;
    }

    fila->inicio = NULL;
    fila->fim = NULL;
}

/* Aloca um pedido e o acrescenta ao fim, preservando a ordem FIFO. */
void enfileirar(Fila *fila, int setor, int codigoMaterial, int quantidade) {
    if (fila == NULL || quantidade <= 0) {
        fprintf(stderr, "Erro: fila invalida ou quantidade nao positiva.\n");
        return;
    }

    Requisicao *nova = malloc(sizeof(*nova));
    if (nova == NULL) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria para a requisicao.\n");
        return;
    }

    nova->setor = setor;
    nova->codigoMaterial = codigoMaterial;
    nova->quantidade = quantidade;
    nova->prox = NULL;

    /* Se estava vazia, o novo pedido é também o primeiro da fila. */
    if (fila->fim == NULL) {
        fila->inicio = nova;
    } else {
        fila->fim->prox = nova;
    }
    fila->fim = nova;
}

/* Consulta o pedido mais antigo sem removê-lo; retorna NULL se não houver. */
Requisicao *consultarInicio(Fila *fila) {
    if (fila == NULL) {
        return NULL;
    }

    return fila->inicio;
}

/* Retira e libera o pedido mais antigo, ajustando o fim se a fila esvaziar. */
void desenfileirar(Fila *fila) {
    if (fila == NULL || fila->inicio == NULL) {
        return;
    }

    Requisicao *removida = fila->inicio;
    fila->inicio = removida->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(removida);
}

/* Mostra todos os pedidos pendentes, do mais antigo ao mais recente. */
void listarRequisicoes(Fila *fila) {
    if (fila == NULL || fila->inicio == NULL) {
        puts("Fila de requisicoes vazia.");
        return;
    }

    for (Requisicao *atual = fila->inicio; atual != NULL; atual = atual->prox) {
        printf("Setor %d | Material %d | Quantidade: %d\n",
               atual->setor, atual->codigoMaterial, atual->quantidade);
    }
}

/* Esvazia a fila liberando a memória de cada pedido. */
void liberarFila(Fila *fila) {
    if (fila == NULL) {
        return;
    }

    while (fila->inicio != NULL) {
        desenfileirar(fila);
    }
}
