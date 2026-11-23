# Sistema simplificado de almoxarifado

Projeto em C para controlar materiais de um almoxarifado e as requisições feitas
por diferentes setores.

O programa mantém os materiais em uma árvore binária de busca (ABB) e organiza
as requisições em uma fila FIFO, processando-as na ordem em que foram
registradas e atualizando o estoque conforme os pedidos são atendidos.

O menu fica separado do ponto de entrada: `main.c` prepara e libera os dados,
enquanto `src/menu.c` conduz a interação com a pessoa usuária.

Consulte [INSTRUCOES.md](INSTRUCOES.md) para compilar, executar e usar o programa.
O [guia de funções C](GUIA_FUNCOES_C.md) explica as funções da biblioteca
padrão e as funções próprias do projeto.