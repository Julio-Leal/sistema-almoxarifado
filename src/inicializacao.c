#include "material.h"

/* Carrega dados iniciais de exemplo. Para na primeira falha para o chamador
   liberar os materiais que já foram inseridos. */
int carregarMateriaisIniciais(NoMaterial **raiz) {
    return inserirMaterialComResultado(raiz, 50, "Papel A4 (resma)", 20) &&
           inserirMaterialComResultado(raiz, 30, "Caneta azul", 50) &&
           inserirMaterialComResultado(raiz, 70, "Toner para impressora", 5) &&
           inserirMaterialComResultado(raiz, 20, "Lapis", 40) &&
           inserirMaterialComResultado(raiz, 40, "Borracha", 25) &&
           inserirMaterialComResultado(raiz, 60, "Pasta de arquivo", 15) &&
           inserirMaterialComResultado(raiz, 80, "Grampeador", 8);
}
