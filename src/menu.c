#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"

/* Lê um inteiro válido. Grava o valor em *valor ou retorna 0 ao fim da entrada. */
static int lerInteiro(const char *mensagem, int *valor) {
    char linha[128];

    for (;;) {
        fputs(mensagem, stdout);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            return 0;
        }

        size_t tamanho = strlen(linha);
        if (tamanho > 0 && linha[tamanho - 1] == '\n') {
            linha[tamanho - 1] = '\0';
        } else if (!feof(stdin)) {
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
            puts("Entrada muito longa. Tente novamente.");
            continue;
        }

        errno = 0;
        char *fim;
        long numero = strtol(linha, &fim, 10);
        if (linha == fim) {
            puts("Entrada invalida. Digite um numero inteiro.");
            continue;
        }
        while (isspace((unsigned char)*fim)) {
            fim++;
        }

        if (*fim != '\0' || errno == ERANGE || numero < INT_MIN ||
            numero > INT_MAX) {
            puts("Entrada invalida. Digite um numero inteiro.");
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

/* Lê uma quantidade em *quantidade e só aceita valores a partir do mínimo. */
static int lerQuantidade(const char *mensagem, int quantidadeMinima,
                         int *quantidade) {
    for (;;) {
        if (!lerInteiro(mensagem, quantidade)) {
            return 0;
        }

        if (*quantidade >= quantidadeMinima) {
            return 1;
        }

        printf("A quantidade deve ser maior ou igual a %d.\n", quantidadeMinima);
    }
}

/* Lê um nome sem espaços nas extremidades; retorna 0 se a entrada terminar. */
static int lerNome(char nome[], size_t capacidade) {
    for (;;) {
        fputs("Nome do material: ", stdout);
        if (fgets(nome, (int)capacidade, stdin) == NULL) {
            return 0;
        }

        size_t tamanho = strlen(nome);
        if (tamanho > 0 && nome[tamanho - 1] == '\n') {
            nome[tamanho - 1] = '\0';
        } else if (!feof(stdin)) {
            int caractere = getchar();
            if (caractere != '\n' && caractere != EOF) {
                while ((caractere = getchar()) != '\n' && caractere != EOF) {
                }
                puts("Nome muito longo. Tente novamente.");
                continue;
            }
        }

        char *inicio = nome;
        while (isspace((unsigned char)*inicio)) {
            inicio++;
        }

        char *fim = inicio + strlen(inicio);
        while (fim > inicio && isspace((unsigned char)fim[-1])) {
            fim--;
        }
        *fim = '\0';

        if (inicio == fim) {
            puts("O nome nao pode ficar vazio.");
            continue;
        }

        memmove(nome, inicio, strlen(inicio) + 1);
        return 1;
    }
}

/* Atualiza o estoque na árvore; retorna 0 se a entrada terminar, 1 caso contrário. */
static int cadastrarMaterial(NoMaterial **raiz) {
    int codigo;
    if (!lerInteiro("Codigo do material: ", &codigo)) {
        return 0;
    }

    NoMaterial *material = buscarMaterial(*raiz, codigo);
    int quantidade;

    /* Para um código já cadastrado, a nova quantidade é somada ao estoque. */
    if (material != NULL) {
        if (!lerQuantidade("Quantidade a acrescentar: ", 1, &quantidade)) {
            return 0;
        }

        if (quantidade > INT_MAX - material->estoque) {
            puts("Erro: o estoque excede o limite permitido.");
            return 1;
        }

        if (!inserirMaterialComResultado(raiz, codigo, material->nome,
                                         quantidade)) {
            return 1;
        }
        puts("Estoque atualizado com sucesso.");
        return 1;
    }

    char nome[100];
    if (!lerNome(nome, sizeof(nome)) ||
        !lerQuantidade("Quantidade inicial em estoque: ", 0, &quantidade)) {
        return 0;
    }

    if (!inserirMaterialComResultado(raiz, codigo, nome, quantidade)) {
        return 1;
    }
    material = buscarMaterial(*raiz, codigo);
    if (material == NULL) {
        return 1;
    }

    puts("Material cadastrado com sucesso.");
    return 1;
}

/* Procura pelo código informado e mostra os dados, se o material existir. */
static int buscarEExibirMaterial(NoMaterial *raiz) {
    int codigo;
    if (!lerInteiro("Codigo do material: ", &codigo)) {
        return 0;
    }

    NoMaterial *material = buscarMaterial(raiz, codigo);
    if (material == NULL) {
        puts("Material nao encontrado.");
    } else {
        printf("Codigo: %d | Material: %s | Estoque: %d\n",
               material->codigo, material->nome, material->estoque);
    }
    return 1;
}

/* Lê setor, material e quantidade para a fila; retorna 0 se a entrada terminar. */
static int registrarRequisicao(Fila *fila) {
    int setor;
    int codigoMaterial;
    int quantidade;

    if (!lerInteiro("Codigo do setor: ", &setor) ||
        !lerInteiro("Codigo do material: ", &codigoMaterial) ||
        !lerQuantidade("Quantidade solicitada: ", 1, &quantidade)) {
        return 0;
    }

    Requisicao *fimAnterior = fila->fim;
    enfileirar(fila, setor, codigoMaterial, quantidade);
    /* A fila só muda de tamanho se a alocação da nova requisição funcionar. */
    if (fila->fim == fimAnterior) {
        return 1;
    }

    puts("Requisicao registrada com sucesso!");
    return 1;
}

/* Apresenta as operações disponíveis para a pessoa usuária. */
static void exibirMenu(void) {
    puts("\n===== SISTEMA DE ALMOXARIFADO =====");
    puts("1 - Inserir ou atualizar material no estoque");
    puts("2 - Buscar material por codigo");
    puts("3 - Listar todos os materiais do estoque");
    puts("4 - Registrar requisicao de material");
    puts("5 - Mostrar requisicoes pendentes");
    puts("6 - Processar requisicoes");
    puts("0 - Sair");
}

/* Exibe o menu e encaminha cada opção para a operação correspondente. */
void executarMenu(NoMaterial **raiz, Fila *fila) {
    int continuar = 1;
    while (continuar) {
        int opcao;
        exibirMenu();
        if (!lerInteiro("Opcao: ", &opcao)) {
            break;
        }

        switch (opcao) {
            case 1:
                continuar = cadastrarMaterial(raiz);
                break;
            case 2:
                continuar = buscarEExibirMaterial(*raiz);
                break;
            case 3:
                listarMateriais(*raiz);
                break;
            case 4:
                continuar = registrarRequisicao(fila);
                break;
            case 5:
                listarRequisicoes(fila);
                break;
            case 6:
                processarRequisicoes(*raiz, fila);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                puts("Opcao invalida.");
                break;
        }
    }
}
