# Instruções de uso

## Requisitos

- GCC instalado e disponível no `PATH`.
- Windows PowerShell para executar o programa de testes.

## Compilar e executar pelo terminal

Abra o PowerShell na pasta raiz do projeto e compile:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -I include main.c src\menu.c src\arvore.c src\fila.c src\processamento.c src\inicializacao.c -o sistema-almoxarifado.exe
```

Inicie o programa:

```powershell
.\sistema-almoxarifado.exe
```

## Usar pelo VS Code

1. Abra a pasta do projeto no VS Code.
2. Pressione **Ctrl+Shift+B** e selecione **Compilar projeto**.
3. Para iniciar, abra **Terminal → Executar Tarefa** e selecione **Executar projeto**.
4. Para verificar os testes, selecione **Executar testes** no mesmo menu.

As tarefas de build, execução e teste estão configuradas em `.vscode/tasks.json`.

Para entender as funções de C e as funções próprias do projeto, consulte o
[guia de funções C](GUIA_FUNCOES_C.md).

## Operações disponíveis

Use o número da opção exibida no menu:

1. Cadastrar material novo ou acrescentar unidades a um material existente.
2. Buscar material pelo código.
3. Listar o estoque em ordem crescente de código.
4. Registrar uma requisição informando setor, material e quantidade.
5. Consultar requisições pendentes na ordem de chegada.
6. Processar a fila: cada pedido é atendido integralmente se houver estoque; caso contrário, é recusado. Ao final, o programa exibe o resumo.
0. Sair do programa.

Os materiais predefinidos são carregados ao iniciar. O estoque e as requisições
existem apenas durante a execução; ao encerrar o programa, as alterações não
são salvas.

## Executar os testes

No PowerShell, na raiz do projeto:

```powershell
.\tests\executar-testes.ps1
```

O script compila e executa testes da ABB, da fila, do processamento, de falhas
de alocação e de entradas inválidas. O roteiro manual complementar está em
[tests/roteiro-testes.md](tests/roteiro-testes.md).
