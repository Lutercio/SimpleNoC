@echo off
REM =================================================================
REM Script de Testes Automatizados - Simulação NoC
REM =================================================================

echo === Iniciando Testes Automatizados NoC ===
echo.

REM Criar pasta de outputs se não existir
if not exist "test_outputs" mkdir test_outputs

REM Compilar o projeto
echo [1/4] Compilando projeto...
mingw32-make
if errorlevel 1 (
    echo ERRO: Falha na compilacao!
    pause
    exit /b 1
)

echo [2/4] Executando testes com diferentes configuracoes...

REM Obter timestamp simples usando PowerShell
for /f %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd_HH-mm-ss'"') do set timestamp=%%a

REM Teste 1: XY Routing - Taxa 5%
echo   - Teste XY (5%% injecao)...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 > "test_outputs\test_XY_5pct_%timestamp%.txt"

REM Teste 2: XY Routing - Taxa 10%
echo   - Teste XY (10%% injecao)...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 > "test_outputs\test_XY_10pct_%timestamp%.txt"

REM Teste 3: XY Routing - Taxa 20%
echo   - Teste XY (20%% injecao)...
.\bin\noc_simulation.exe -routing XY -rate 20 -time 200 > "test_outputs\test_XY_20pct_%timestamp%.txt"

REM Teste 4: West-First Routing - Taxa 5%
echo   - Teste West-First (5%% injecao)...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 > "test_outputs\test_WestFirst_5pct_%timestamp%.txt"

REM Teste 5: West-First Routing - Taxa 10%
echo   - Teste West-First (10%% injecao)...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 > "test_outputs\test_WestFirst_10pct_%timestamp%.txt"

REM Teste 6: West-First Routing - Taxa 20%
echo   - Teste West-First (20%% injecao)...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 20 -time 200 > "test_outputs\test_WestFirst_20pct_%timestamp%.txt"

echo [3/4] Todos os testes executados!

REM Verificar se os arquivos foram criados
echo [4/4] Verificando resultados...
set file_count=0
for %%f in (test_outputs\*.txt) do set /a file_count+=1

echo.
echo === Testes Concluidos ===
echo Arquivos gerados: %file_count%
echo Pasta de resultados: test_outputs\
echo.
echo Para analisar os resultados, execute:
echo   mingw32-make analyze
echo.
