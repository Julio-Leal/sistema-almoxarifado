# Roteiro de testes do almoxarifado

Execute os testes automatizados com `.\tests\executar-testes.ps1`.

## Árvore binária de busca

- [x] Inserir materiais em diferentes posições da árvore.
- [x] Atualizar o estoque de um código existente sem criar outro nó.
- [x] Buscar códigos existentes e inexistentes.
- [x] Listar os materiais em ordem crescente de código.
- [x] Simular falha de alocação na raiz e em uma subárvore sem perder os nós existentes.
- [x] Interromper a carga inicial em uma falha de alocação e liberar os materiais carregados parcialmente.
- [x] Liberar a árvore ao final do teste.

## Fila encadeada

- [x] Consultar e listar uma fila vazia.
- [x] Inserir várias requisições e verificar a ordem FIFO e seus campos.
- [x] Rejeitar quantidades zero e negativas.
- [x] Remover requisições até esvaziar a fila e confirmar início/fim nulos.
- [x] Liberar e reutilizar a fila.
- [x] Simular falha de alocação ao enfileirar sem alterar a fila existente.

## Processamento

- [x] Atender requisições integralmente quando há estoque suficiente.
- [x] Recusar sem atendimento parcial quando o estoque é insuficiente.
- [x] Recusar requisições para materiais inexistentes.
- [x] Confirmar atualização do estoque e remoção de todas as requisições.
- [x] Processar novamente com estoque atualizado e com fila vazia.

## Verificação manual do menu

- [x] Cadastrar material novo e atualizar material existente.
- [x] Buscar material existente e inexistente; listar o estoque.
- [x] Registrar e listar requisições antes de processá-las.
- [x] Processar requisições e confirmar o resumo e o retorno ao menu.
- [x] Informar entradas não numéricas, quantidades inválidas e opção inexistente.
- [x] Tratar nome vazio, nome longo, linha numérica longa e fim de entrada (EOF).
- [x] Sair e confirmar encerramento normal.