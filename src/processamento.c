#include <stdio.h>

#include "material.h"

/* Processa cada pedido em ordem; desconta o estoque ou informa a recusa. */
void processarRequisicoes(NoMaterial *raiz, Fila *fila) {
    if (fila == NULL) {
        fprintf(stderr, "Erro: fila invalida.\n");
        return;
    }

    int processadas = 0;
    int atendidas = 0;
    int recusadas = 0;

    puts("===== PROCESSAMENTO DAS REQUISICOES =====");

    Requisicao *requisicao;
    /* A fila é FIFO: cada tentativa consome o pedido, mesmo se ele for recusado. */
    while ((requisicao = consultarInicio(fila)) != NULL) {
        NoMaterial *material = buscarMaterial(raiz, requisicao->codigoMaterial);

        processadas++;
        printf("Setor %d | Material %d", requisicao->setor,
               requisicao->codigoMaterial);

        if (material == NULL) {
            puts("\nRECUSADA - Material inexistente.");
            recusadas++;
        } else {
            printf(" - %s\n", material->nome);
            printf("Solicitado: %d | Disponivel: %d\n",
                   requisicao->quantidade, material->estoque);

            if (material->estoque >= requisicao->quantidade) {
                /* Só pedidos com saldo suficiente alteram o estoque. */
                material->estoque -= requisicao->quantidade;
                printf("ATENDIDA - Estoque restante: %d\n", material->estoque);
                atendidas++;
            } else {
                puts("RECUSADA - Estoque insuficiente.");
                recusadas++;
            }
        }

        desenfileirar(fila);
    }

    puts("===== RESUMO DO PROCESSAMENTO =====");
    printf("Processadas: %d | Atendidas: %d | Recusadas: %d\n",
           processadas, atendidas, recusadas);
}
