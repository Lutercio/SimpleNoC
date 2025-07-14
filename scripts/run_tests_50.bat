@echo off
REM =================================================================
REM Script de Testes Automatizados Expandido - Simulação NoC
REM 50 Testes com malhas de 4x4 até 16x16
REM =================================================================

echo === Iniciando Testes Automatizados Expandidos (50 testes) ===
echo.

REM Criar pasta de outputs se não existir
if not exist "test_outputs" mkdir test_outputs

REM Compilar o projeto
echo [1/3] Compilando projeto...
mingw32-make
if errorlevel 1 (
    echo ERRO: Falha na compilacao!
    pause
    exit /b 1
)

echo [2/3] Executando 50 testes com diferentes configuracoes...

REM Obter timestamp simples usando PowerShell
for /f %%a in ('powershell -Command "Get-Date -Format 'yyyy-MM-dd_HH-mm-ss'"') do set timestamp=%%a

set test_count=0

echo.
echo === MALHA 4x4 (16 testes) ===

REM Malha 4x4 - Taxa 5%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 5%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 -size 4 > "test_outputs\test_XY_4x4_5pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 5%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_5pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 10%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 10%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 -size 4 > "test_outputs\test_XY_4x4_10pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 10%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_10pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 15%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 15%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 15 -time 200 -size 4 > "test_outputs\test_XY_4x4_15pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 15%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 15 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_15pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 20%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 20%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 20 -time 200 -size 4 > "test_outputs\test_XY_4x4_20pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 20%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 20 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_20pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 25%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 25%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 25 -time 200 -size 4 > "test_outputs\test_XY_4x4_25pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 25%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 25 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_25pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 30%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 30%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 30 -time 200 -size 4 > "test_outputs\test_XY_4x4_30pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 30%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 30 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_30pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 35%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 35%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 35 -time 200 -size 4 > "test_outputs\test_XY_4x4_35pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 35%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 35 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_35pct_%timestamp%.txt"

REM Malha 4x4 - Taxa 40%
set /a test_count+=1
echo   [%test_count%/50] XY 4x4 - 40%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 40 -time 200 -size 4 > "test_outputs\test_XY_4x4_40pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 4x4 - 40%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 40 -time 200 -size 4 > "test_outputs\test_WestFirst_4x4_40pct_%timestamp%.txt"

echo.
echo === MALHA 6x6 (12 testes) ===

REM Malha 6x6 - Taxa 5%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 5%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 -size 6 > "test_outputs\test_XY_6x6_5pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 5%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_5pct_%timestamp%.txt"

REM Malha 6x6 - Taxa 10%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 10%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 -size 6 > "test_outputs\test_XY_6x6_10pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 10%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_10pct_%timestamp%.txt"

REM Malha 6x6 - Taxa 15%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 15%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 15 -time 200 -size 6 > "test_outputs\test_XY_6x6_15pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 15%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 15 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_15pct_%timestamp%.txt"

REM Malha 6x6 - Taxa 20%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 20%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 20 -time 200 -size 6 > "test_outputs\test_XY_6x6_20pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 20%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 20 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_20pct_%timestamp%.txt"

REM Malha 6x6 - Taxa 25%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 25%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 25 -time 200 -size 6 > "test_outputs\test_XY_6x6_25pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 25%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 25 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_25pct_%timestamp%.txt"

REM Malha 6x6 - Taxa 30%
set /a test_count+=1
echo   [%test_count%/50] XY 6x6 - 30%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 30 -time 200 -size 6 > "test_outputs\test_XY_6x6_30pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 6x6 - 30%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 30 -time 200 -size 6 > "test_outputs\test_WestFirst_6x6_30pct_%timestamp%.txt"

echo.
echo === MALHA 8x8 (12 testes) ===

REM Malha 8x8 - Taxa 5%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 5%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 -size 8 > "test_outputs\test_XY_8x8_5pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 5%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_5pct_%timestamp%.txt"

REM Malha 8x8 - Taxa 10%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 10%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 -size 8 > "test_outputs\test_XY_8x8_10pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 10%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_10pct_%timestamp%.txt"

REM Malha 8x8 - Taxa 15%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 15%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 15 -time 200 -size 8 > "test_outputs\test_XY_8x8_15pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 15%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 15 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_15pct_%timestamp%.txt"

REM Malha 8x8 - Taxa 20%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 20%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 20 -time 200 -size 8 > "test_outputs\test_XY_8x8_20pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 20%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 20 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_20pct_%timestamp%.txt"

REM Malha 8x8 - Taxa 25%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 25%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 25 -time 200 -size 8 > "test_outputs\test_XY_8x8_25pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 25%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 25 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_25pct_%timestamp%.txt"

REM Malha 8x8 - Taxa 30%
set /a test_count+=1
echo   [%test_count%/50] XY 8x8 - 30%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 30 -time 200 -size 8 > "test_outputs\test_XY_8x8_30pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 8x8 - 30%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 30 -time 200 -size 8 > "test_outputs\test_WestFirst_8x8_30pct_%timestamp%.txt"

echo.
echo === MALHA 12x12 (6 testes) ===

REM Malha 12x12 - Taxa 5%
set /a test_count+=1
echo   [%test_count%/50] XY 12x12 - 5%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 -size 12 > "test_outputs\test_XY_12x12_5pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 12x12 - 5%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 -size 12 > "test_outputs\test_WestFirst_12x12_5pct_%timestamp%.txt"

REM Malha 12x12 - Taxa 10%
set /a test_count+=1
echo   [%test_count%/50] XY 12x12 - 10%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 -size 12 > "test_outputs\test_XY_12x12_10pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 12x12 - 10%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 -size 12 > "test_outputs\test_WestFirst_12x12_10pct_%timestamp%.txt"

REM Malha 12x12 - Taxa 15%
set /a test_count+=1
echo   [%test_count%/50] XY 12x12 - 15%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 15 -time 200 -size 12 > "test_outputs\test_XY_12x12_15pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 12x12 - 15%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 15 -time 200 -size 12 > "test_outputs\test_WestFirst_12x12_15pct_%timestamp%.txt"

echo.
echo === MALHA 16x16 (4 testes) ===

REM Malha 16x16 - Taxa 5%
set /a test_count+=1
echo   [%test_count%/50] XY 16x16 - 5%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 5 -time 200 -size 16 > "test_outputs\test_XY_16x16_5pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 16x16 - 5%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 5 -time 200 -size 16 > "test_outputs\test_WestFirst_16x16_5pct_%timestamp%.txt"

REM Malha 16x16 - Taxa 10%
set /a test_count+=1
echo   [%test_count%/50] XY 16x16 - 10%% injecao...
.\bin\noc_simulation.exe -routing XY -rate 10 -time 200 -size 16 > "test_outputs\test_XY_16x16_10pct_%timestamp%.txt"

set /a test_count+=1
echo   [%test_count%/50] West-First 16x16 - 10%% injecao...
.\bin\noc_simulation.exe -routing WEST_FIRST -rate 10 -time 200 -size 16 > "test_outputs\test_WestFirst_16x16_10pct_%timestamp%.txt"

echo.
echo [3/3] Todos os 50 testes executados!

REM Verificar se os arquivos foram criados
echo === Verificando resultados...
set file_count=0
for %%f in (test_outputs\*.txt) do set /a file_count+=1

echo.
echo === Testes Expandidos Concluidos ===
echo Total de testes executados: 50
echo Arquivos gerados: %file_count%
echo Pasta de resultados: test_outputs\
echo.
echo Configurações testadas:
echo   - Malhas: 4x4 (16 testes), 6x6 (12 testes), 8x8 (12 testes), 12x12 (6 testes), 16x16 (4 testes)
echo   - Taxas de injeção: 5%%, 10%%, 15%%, 20%%, 25%%, 30%%, 35%%, 40%%
echo   - Algoritmos: XY Routing e West-First
echo.
echo Para analisar os resultados expandidos, execute:
echo   mingw32-make analyze-extended
echo.
pause
