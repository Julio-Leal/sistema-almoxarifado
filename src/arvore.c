#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "material.h"

/* Insere pelo código: menores ficam à esquerda e maiores à direita na ABB. */
static int inserirNo(NoMaterial **raiz, int codigo, const char nome[],
                     int estoque) {
    if (*raiz == NULL) {
        /* O nó só é ligado à árvore depois que a alocação tem sucesso. */
        NoMaterial *novo = malloc(sizeof(*novo));
        if (novo == NULL) {
            fprintf(stderr, "Erro: nao foi possivel alocar memoria para o material.\n");
            return 0;
        }

        novo->codigo = codigo;
        snprintf(novo->nome, sizeof(novo->nome), "%s", nome);
        novo->estoque = estoque;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    if (codigo < (*raiz)->codigo) {
        return inserirNo(&(*raiz)->esq, codigo, nome, estoque);
    }

    if (codigo > (*raiz)->codigo) {
        return inserirNo(&(*raiz)->dir, codigo, nome, estoque);
    }

    /* Códigos repetidos representam o mesmo material e acumulam o estoque. */
    if (estoque > INT_MAX - (*raiz)->estoque) {
        fprintf(stderr, "Erro: o estoque excede o limite permitido.\n");
        return 0;
    }

    (*raiz)->estoque += estoque;
    return 1;
}

/* Valida os dados e inicia a inserção; retorna 1 em sucesso e 0 em falha. */
int inserirMaterialComResultado(NoMaterial **raiz, int codigo,
                                const char nome[], int estoque) {
    if (raiz == NULL || estoque < 0 || nome == NULL) {
        fprintf(stderr, "Erro: nome ou estoque do material invalido.\n");
        return 0;
    }

    return inserirNo(raiz, codigo, nome, estoque);
}

/* Versão que devolve a raiz, para distinguir a falha usar a outra função. */
NoMaterial *inserirMaterial(NoMaterial *raiz, int codigo,
                            const char nome[], int estoque) {
    inserirMaterialComResultado(&raiz, codigo, nome, estoque);
    return raiz;
}

/* Segue os ramos ordenados pelo código e retorna o nó ou NULL se não houver. */
NoMaterial *buscarMaterial(NoMaterial *raiz, int codigo) {
    if (raiz == NULL || raiz->codigo == codigo) {
        return raiz;
    }

    if (codigo < raiz->codigo) {
        return buscarMaterial(raiz->esq, codigo);
    }

    return buscarMaterial(raiz->dir, codigo);
}

/* Percorre esquerda, nó e direita para exibir os materiais por código crescente. */
void listarMateriais(NoMaterial *raiz) {
    if (raiz == NULL) {
        puts("Estoque vazio.");
        return;
    }

    if (raiz->esq != NULL) {
        listarMateriais(raiz->esq);
    }

    printf("Codigo: %d | Material: %s | Estoque: %d\n",
           raiz->codigo, raiz->nome, raiz->estoque);

    if (raiz->dir != NULL) {
        listarMateriais(raiz->dir);
    }
}

/* Libera os ramos antes do nó para não perder o acesso aos filhos. */
void liberarArvore(NoMaterial *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}