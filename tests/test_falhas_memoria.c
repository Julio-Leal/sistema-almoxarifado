#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "material.h"

static int falharAposAlocacoes = -1;

void *__real_malloc(size_t tamanho);

void *__wrap_malloc(size_t tamanho) {
    if (falharAposAlocacoes == 0) {
        falharAposAlocacoes = -1;
        return NULL;
    }

    if (falharAposAlocacoes > 0) {
        falharAposAlocacoes--;
    }

    return __real_malloc(tamanho);
}

static int contarNos(NoMaterial *raiz) {
    if (raiz == NULL) {
        return 0;
    }

    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

int main(void) {
    NoMaterial *raiz = NULL;

    for (int alocacaoComFalha = 0; alocacaoComFalha < 7; alocacaoComFalha++) {
        raiz = NULL;
        falharAposAlocacoes = alocacaoComFalha;
        assert(!carregarMateriaisIniciais(&raiz));
        assert(contarNos(raiz) == alocacaoComFalha);
        assert((buscarMaterial(raiz, 70) != NULL) == (alocacaoComFalha >= 3));
        liberarArvore(raiz);
    }

    raiz = inserirMaterial(NULL, 50, "Papel A4", 8);
    assert(raiz != NULL);
    assert(inserirMaterialComResultado(&raiz, 30, "Caneta", 5));
    NoMaterial *subarvoreEsquerda = raiz->esq;

    falharAposAlocacoes = 0;
    assert(!inserirMaterialComResultado(&raiz, 20, "Lapiz", 3));
    assert(raiz->esq == subarvoreEsquerda);
    assert(buscarMaterial(raiz, 30) != NULL);
    assert(buscarMaterial(raiz, 20) == NULL);
    liberarArvore(raiz);

    raiz = inserirMaterial(NULL, 50, "Papel A4", 8);
    assert(inserirMaterialComResultado(&raiz, 70, "Toner", 5));
    NoMaterial *subarvoreDireita = raiz->dir;

    falharAposAlocacoes = 0;
    assert(!inserirMaterialComResultado(&raiz, 80, "Grampeador", 2));
    assert(raiz->dir == subarvoreDireita);
    assert(buscarMaterial(raiz, 70) != NULL);
    assert(buscarMaterial(raiz, 80) == NULL);
    liberarArvore(raiz);

    Fila fila;
    inicializarFila(&fila);
    enfileirar(&fila, 10, 50, 2);
    Requisicao *fimOriginal = fila.fim;

    falharAposAlocacoes = 0;
    enfileirar(&fila, 20, 70, 1);
    assert(fila.fim == fimOriginal);
    assert(fila.inicio == fimOriginal);
    assert(fila.fim->prox == NULL);
    liberarFila(&fila);

    puts("Testes de falha de memoria passaram.");
    return 0;
}
