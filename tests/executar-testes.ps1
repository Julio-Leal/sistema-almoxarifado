$ErrorActionPreference = 'Stop'
$raizProjeto = Split-Path -Parent $PSScriptRoot
$executaveisTemporarios = @()
Push-Location $raizProjeto

try {
    $casos = @(
        @{
            Nome = 'ABB'
            Fontes = @('tests\test_arvore.c', 'src\arvore.c')
        },
        @{
            Nome = 'Fila'
            Fontes = @('tests\test_fila.c', 'src\fila.c')
        },
        @{
            Nome = 'Processamento'
            Fontes = @(
                'tests\test_processamento.c',
                'src\arvore.c',
                'src\fila.c',
                'src\processamento.c'
            )
            SaidasEsperadas = @(
                '(?s)Setor 10 \| Material 50 - Papel A4.*?Setor 20 \| Material 50 - Papel A4.*?Setor 30 \| Material 50 - Papel A4.*?Setor 40 \| Material 999',
                'Processadas: 4 \| Atendidas: 2 \| Recusadas: 2',
                'Processadas: 1 \| Atendidas: 1 \| Recusadas: 0',
                'Processadas: 0 \| Atendidas: 0 \| Recusadas: 0'
            )
        },
        @{
            Nome = 'Falhas de memoria'
            Fontes = @(
                'tests\test_falhas_memoria.c',
                'src\arvore.c',
                'src\fila.c',
                'src\inicializacao.c'
            )
            ArgumentosLinker = @('-Wl,--wrap=malloc')
            SaidasEsperadas = @(
                'Testes de falha de memoria passaram\.'
            )
        }
    )

    foreach ($caso in $casos) {
        $executavel = Join-Path $env:TEMP ("almoxarifado-{0}.exe" -f [guid]::NewGuid())
        $executaveisTemporarios += $executavel
        Write-Output "===== Testes: $($caso.Nome) ====="

        & gcc -std=c11 -Wall -Wextra -Wpedantic -I include @($caso.Fontes) @($caso.ArgumentosLinker) -o $executavel
        if ($LASTEXITCODE -ne 0) {
            throw "Falha ao compilar os testes de $($caso.Nome)."
        }

        $preferenciaErro = $ErrorActionPreference
        $ErrorActionPreference = 'Continue'
        $saida = & $executavel 2>&1 | Out-String
        $codigoSaida = $LASTEXITCODE
        $ErrorActionPreference = $preferenciaErro
        if ($codigoSaida -ne 0) {
            throw "Falha nos testes de $($caso.Nome)."
        }

        foreach ($esperada in $caso.SaidasEsperadas) {
            if ($saida -notmatch $esperada) {
                throw "Saida esperada ausente nos testes de $($caso.Nome): $esperada"
            }
        }

        if ($caso.Nome -eq 'Processamento') {
            Write-Output $saida
        }

        Remove-Item -LiteralPath $executavel -Force
    }

    $executavel = Join-Path $env:TEMP ("almoxarifado-menu-{0}.exe" -f [guid]::NewGuid())
    $executaveisTemporarios += $executavel
    & gcc -std=c11 -Wall -Wextra -Wpedantic -I include main.c src\menu.c src\arvore.c src\fila.c src\processamento.c src\inicializacao.c -o $executavel
    if ($LASTEXITCODE -ne 0) {
        throw 'Falha ao compilar o programa completo.'
    }

    $nomeLongo = 'N' * 101
    $linhaNumericaLonga = '1' * 130
    $entrada = @(
        'abc', '7',
        '1', '50', '2147483647',
        '2', '50',
        '2', '999',
        '4', '10', '50', '0', '2',
        '5', '6',
        '2', '50',
        '1', '50', '2',
        '1', '90', 'Clipes', '12',
        '2', '90',
        '3',
        '1', '92', ('M' * 99), '0',
        '2', '92',
        '1', '91', '', $nomeLongo, 'Marcador valido', '0',
        '2', '91',
        $linhaNumericaLonga, '0',
        '0'
    )
    $preferenciaErro = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $saidaMenu = $entrada | & $executavel 2>&1 | Out-String
    $codigoSaidaMenu = $LASTEXITCODE
    $ErrorActionPreference = $preferenciaErro
    if ($codigoSaidaMenu -ne 0) {
        throw 'Falha durante o teste do menu.'
    }

    $saidasEsperadasMenu = @(
        'Entrada invalida\. Digite um numero inteiro\.',
        'Opcao invalida\.',
        'Erro: o estoque excede o limite permitido\.',
        'Codigo: 50 \| Material: Papel A4 \(resma\) \| Estoque: 20',
        'Material nao encontrado\.',
        'A quantidade deve ser maior ou igual a 1\.',
        'Setor 10 \| Material 50 \| Quantidade: 2',
        'Processadas: 1 \| Atendidas: 1 \| Recusadas: 0',
        'Codigo: 50 \| Material: Papel A4 \(resma\) \| Estoque: 18',
        'Estoque atualizado com sucesso\.',
        'Material cadastrado com sucesso\.',
        'Codigo: 90 \| Material: Clipes \| Estoque: 12',
        'Codigo: 20 \| Material: Lapis \| Estoque: 40',
        ('Codigo: 92 \| Material: ' + ('M' * 99) + ' \| Estoque: 0'),
        'O nome nao pode ficar vazio\.',
        'Nome muito longo\. Tente novamente\.',
        'Codigo: 91 \| Material: Marcador valido \| Estoque: 0',
        'Entrada muito longa\. Tente novamente\.'
    )
    foreach ($esperada in $saidasEsperadasMenu) {
        if ($saidaMenu -notmatch $esperada) {
            throw "Saida esperada ausente no teste do menu: $esperada"
        }
    }

    Write-Output 'Testes do menu passaram.'

    $preferenciaErro = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    $saidaEOF = @() | & $executavel 2>&1 | Out-String
    $codigoSaidaEOF = $LASTEXITCODE
    $ErrorActionPreference = $preferenciaErro
    if ($codigoSaidaEOF -ne 0 -or $saidaEOF -notmatch '===== SISTEMA DE ALMOXARIFADO =====') {
        throw 'Falha ao encerrar corretamente quando a entrada termina (EOF).'
    }

    $saidaOpcaoSair = @('0', 'entrada que deve ser ignorada') | & $executavel 2>&1 | Out-String
    if ($LASTEXITCODE -ne 0 -or $saidaOpcaoSair -match 'Entrada invalida') {
        throw 'A opção 0 não encerrou o menu imediatamente.'
    }

    $saidaEOFDuranteOperacao = @('1', '999') | & $executavel 2>&1 | Out-String
    if ($LASTEXITCODE -ne 0 -or $saidaEOFDuranteOperacao -notmatch 'Nome do material:') {
        throw 'Falha ao encerrar corretamente quando a entrada termina durante o cadastro.'
    }

    $saidaEOFDuranteNumero = @('2') | & $executavel 2>&1 | Out-String
    if ($LASTEXITCODE -ne 0 -or $saidaEOFDuranteNumero -notmatch 'Codigo do material:') {
        throw 'Falha ao encerrar corretamente quando a entrada termina durante a leitura numerica.'
    }

    Remove-Item -LiteralPath $executavel -Force

    Write-Output 'Teste de EOF passou.'
    Write-Output 'Teste da opção de saída passou.'
    Write-Output 'Todos os testes passaram.'
} finally {
    foreach ($executavelTemporario in $executaveisTemporarios) {
        if (Test-Path -LiteralPath $executavelTemporario) {
            Remove-Item -LiteralPath $executavelTemporario -Force
        }
    }
    Pop-Location
}
