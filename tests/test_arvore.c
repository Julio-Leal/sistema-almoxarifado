#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "material.h"

static void verificarOrdemCrescente(NoMaterial *raiz, int *anterior,
                                    int *quantidade) {
    if (raiz == NULL) {
        return;
    }

    verificarOrdemCrescente(raiz->esq, anterior, quantidade);
    assert(raiz->codigo > *anterior);
    *anterior = raiz->codigo;
    (*quantidade)++;
    verificarOrdemCrescente(raiz->dir, anterior, quantidade);
}

int main(void) {
    NoMaterial *raiz = NULL;
    int anterior = -1;
    int quantidade = 0;

    raiz = inserirMaterial(raiz, 50, "Papel A4", 20);
    raiz = inserirMaterial(raiz, 30, "Caneta azul", 50);
    raiz = inserirMaterial(raiz, 70, "Toner", 5);
    raiz = inserirMaterial(raiz, 20, "Lapis", 40);
    raiz = inserirMaterial(raiz, 40, "Borracha", 25);
    raiz = inserirMaterial(raiz, 60, "Pasta", 15);
    raiz = inserirMaterial(raiz, 80, "Grampeador", 8);
    raiz = inserirMaterial(raiz, 35, "Marcador", 12);
    raiz = inserirMaterial(raiz, 90, "Clipes", 10);

    raiz = inserirMaterial(raiz, 50, "Nome nao substituido", 3);

    assert(raiz != NULL);
    assert(raiz->codigo == 50);
    assert(raiz->estoque == 23);
    assert(buscarMaterial(raiz, 35) != NULL);
    assert(buscarMaterial(raiz, 35)->estoque == 12);
    assert(buscarMaterial(raiz, 999) == NULL);
    assert(buscarMaterial(raiz, 50)->nome[0] == 'P');

    raiz = inserirMaterial(raiz, 100, "Material no limite", INT_MAX);
    assert(raiz != NULL);
    assert(!inserirMaterialComResultado(&raiz, 100, "Outro nome", 1));
    assert(buscarMaterial(raiz, 100)->estoque == INT_MAX);

    verificarOrdemCrescente(raiz, &anterior, &quantidade);
    assert(quantidade == 10);

    puts("Listagem em ordem crescente:");
    listarMateriais(raiz);
    liberarArvore(raiz);

    puts("Testes da ABB passaram.");
    return 0;
}
