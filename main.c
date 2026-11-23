#include <stdlib.h>

#include "material.h"
#include "menu.h"

/* Prepara as estruturas, executa o menu e libera a memória antes de sair. */
int main(void) {
    NoMaterial *raiz = NULL;
    Fila fila;

    inicializarFila(&fila);
    if (!carregarMateriaisIniciais(&raiz)) {
        liberarFila(&fila);
        liberarArvore(raiz);
        return EXIT_FAILURE;
    }

    executarMenu(&raiz, &fila);

    liberarFila(&fila);
    liberarArvore(raiz);
    return 0;
}
