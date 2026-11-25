#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "material.h"

static void verificarOrdemCrescente(NoMaterial *raiz, int *anterior, int *quantidade) {
    if (raiz == NULL) {
        return;
    }
    verificarOrdemCrescente(raiz->esq, anterior, quantidade);
    assert(raiz->codigo > *anterior);
    *anterior = raiz->codigo;
    (*quantidade)++;
    verificarOrdemCrescente(raiz->dir, anterior, quantidade);
}

static void testarArvoreInicial(void) {
    NoMaterial *arvoreInicial = NULL;
    puts("\n===== TESTE DA ARVORE INICIAL =====");
    assert(carregarMateriaisIniciais(&arvoreInicial));
    assert(arvoreInicial != NULL);
    assert(arvoreInicial->codigo == 50);
    assert(arvoreInicial->esq != NULL);
    assert(arvoreInicial->dir != NULL);
    assert(arvoreInicial->esq->codigo == 30);
    assert(arvoreInicial->dir->codigo == 70);
    assert(arvoreInicial->esq->esq != NULL);
    assert(arvoreInicial->esq->dir != NULL);
    assert(arvoreInicial->esq->esq->codigo == 20);
    assert(arvoreInicial->esq->dir->codigo == 40);
    assert(arvoreInicial->dir->esq != NULL);
    assert(arvoreInicial->dir->dir != NULL);
    assert(arvoreInicial->dir->esq->codigo == 60);
    assert(arvoreInicial->dir->dir->codigo == 80);

    int anterior = -1;
    int quantidade = 0;

    verificarOrdemCrescente(arvoreInicial, &anterior, &quantidade);
    assert(quantidade == 7);
    puts("Estrutura da arvore inicial correta!");
    puts("Quantidade de materiais: 7");
    liberarArvore(arvoreInicial);
}

static void testarOperacoesArvore(void) {
    NoMaterial *raiz = NULL;
    int anterior = -1;
    int quantidade = 0;
    puts("\n===== TESTE DAS OPERACOES DA ABB =====");
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
    assert(!inserirMaterialComResultado(
        &raiz, 100, "Outro nome", 1
    ));
    assert(buscarMaterial(raiz, 100)->estoque == INT_MAX);
    verificarOrdemCrescente(raiz, &anterior, &quantidade);
    assert(quantidade == 10);
    puts("\nListagem em ordem crescente:");
    listarMateriais(raiz);
    liberarArvore(raiz);
}

int main(void) {
    testarArvoreInicial();
    testarOperacoesArvore();
    puts("\nTodos os testes da ABB passaram!");
    return 0;
}
