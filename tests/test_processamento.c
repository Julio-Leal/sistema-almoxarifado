#include <assert.h>
#include <stdio.h>

#include "material.h"

int main(void) {
    NoMaterial *raiz = inserirMaterial(NULL, 50, "Papel A4", 8);
    Fila fila;

    inicializarFila(&fila);
    assert(raiz != NULL);

    enfileirar(&fila, 10, 50, 3);
    enfileirar(&fila, 20, 50, 4);
    enfileirar(&fila, 30, 50, 2);
    enfileirar(&fila, 40, 999, 1);

    processarRequisicoes(raiz, &fila);

    assert(buscarMaterial(raiz, 50)->estoque == 1);
    assert(consultarInicio(&fila) == NULL);
    assert(fila.inicio == NULL);
    assert(fila.fim == NULL);

    enfileirar(&fila, 50, 50, 1);
    processarRequisicoes(raiz, &fila);
    assert(buscarMaterial(raiz, 50)->estoque == 0);
    assert(consultarInicio(&fila) == NULL);

    processarRequisicoes(raiz, &fila);

    liberarFila(&fila);
    liberarArvore(raiz);
    puts("Testes do processamento passaram.");
    return 0;
}
