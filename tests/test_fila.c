#include <assert.h>
#include <stdio.h>

#include "material.h"

int main(void) {
    Fila fila;

    inicializarFila(&fila);
    assert(consultarInicio(&fila) == NULL);
    desenfileirar(&fila);
    liberarFila(&fila);

    enfileirar(&fila, 10, 50, 3);
    enfileirar(&fila, 20, 70, 2);
    enfileirar(&fila, 30, 30, 5);
    Requisicao *fimValido = fila.fim;
    enfileirar(&fila, 99, 99, 0);
    enfileirar(&fila, 99, 99, -1);
    assert(fila.fim == fimValido);

    Requisicao *primeira = consultarInicio(&fila);
    assert(primeira != NULL);
    assert(primeira->setor == 10);
    assert(primeira->codigoMaterial == 50);
    assert(primeira->quantidade == 3);
    assert(primeira->prox != NULL);
    assert(primeira->prox->setor == 20);
    assert(primeira->prox->codigoMaterial == 70);
    assert(primeira->prox->quantidade == 2);
    assert(primeira->prox->prox != NULL);
    assert(primeira->prox->prox->setor == 30);
    assert(primeira->prox->prox->codigoMaterial == 30);
    assert(primeira->prox->prox->quantidade == 5);
    assert(primeira->prox->prox->prox == NULL);
    assert(fila.fim == primeira->prox->prox);

    listarRequisicoes(&fila);

    desenfileirar(&fila);
    primeira = consultarInicio(&fila);
    assert(primeira != NULL);
    assert(primeira->setor == 20);
    assert(primeira->codigoMaterial == 70);
    assert(primeira->quantidade == 2);

    desenfileirar(&fila);
    primeira = consultarInicio(&fila);
    assert(primeira != NULL);
    assert(primeira->setor == 30);
    assert(primeira->codigoMaterial == 30);
    assert(primeira->quantidade == 5);

    desenfileirar(&fila);
    assert(consultarInicio(&fila) == NULL);
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);
    listarRequisicoes(&fila);

    enfileirar(&fila, 40, 80, 1);
    assert(consultarInicio(&fila) != NULL);
    assert(consultarInicio(&fila)->setor == 40);
    assert(consultarInicio(&fila)->codigoMaterial == 80);
    assert(consultarInicio(&fila)->quantidade == 1);
    liberarFila(&fila);
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);

    enfileirar(&fila, 50, 50, 2);
    primeira = consultarInicio(&fila);
    assert(primeira != NULL);
    assert(primeira->setor == 50);
    assert(primeira->codigoMaterial == 50);
    assert(primeira->quantidade == 2);
    desenfileirar(&fila);
    assert(consultarInicio(&fila) == NULL);
    assert(fila.fim == NULL);

    puts("Testes da fila passaram.");
    return 0;
}
