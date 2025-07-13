# 🌐 Network-on-Chip (NoC) Simulation with SystemC

## 📋 Descrição do Projeto

Este projeto implementa uma simulação completa de **Rede em Chip (Network-on-Chip)** usando **Sy### 🔬 Teste Manual Específico
```bash
# Malha pequena
.\bin\noc_simulation.exe -size 3 -rate 25

# Malha grande
.\bin\noc_simulation.exe -size 8 -rate 10 -time 2000
```

### 🔬 Análise de Congestionamento
```bash
# Baixa injeção
.\bin\noc_simulation.exe -rate 5

# Alta injeção
.\bin\noc_simulation.exe -rate 50
```vido como trabalho acadêmico para a disciplina de arquitetura de sistemas digitais. 

O sistema simula uma rede em chip com **topologia de malha 2D**, onde pacotes são transmitidos entre diferentes nós usando **dois algoritmos de roteamento distintos**: XY Routing e West-First Routing.

## 🎯 Objetivos Acadêmicos

✅ **Projetar e implementar** um modelo funcional de NoC  
✅ **Implementar dois algoritmos de roteamento** distintos  
✅ **Demonstrar comportamento** e diferenças entre os algoritmos  
✅ **Validar funcionamento** através de simulações  

## 🏗️ Arquitetura do Sistema

### 🔧 Componentes Principais

| Componente | Descrição | Arquivo |
|------------|-----------|---------|
| **🎯 Node** | Gera e consome pacotes com taxa configurável | `node.h` |
| **🔀 Router** | Encaminha pacotes usando algoritmos de roteamento | `router.h` |
| **📡 Channel** | Canais de comunicação entre componentes | `channel.h` |
| **📦 Packet** | Estrutura de dados com origem, destino e payload | `packet.h` |
| **🧠 Routing** | Algoritmos XY e West-First implementados | `routing_algorithms.h` |
| **🌐 NoC** | Coordenação geral da rede em chip | `noc.h` |

### 🎨 Topologia de Rede

- **Tipo**: Malha 2D (mesh topology)
- **Tamanho**: Configurável (2x2 até 16x16)
- **Conexões**: Cada roteador conectado aos vizinhos (Norte, Sul, Leste, Oeste)
- **Nó Local**: Cada roteador possui um nó local conectado

## 🧮 Algoritmos de Roteamento

### 1. 📐 XY Routing (Dimension-Ordered)
```cpp
// Primeiro move no eixo X, depois no eixo Y
if (current_x != dest_x) → move EAST/WEST
else if (current_y != dest_y) → move NORTH/SOUTH
else → LOCAL (destino alcançado)
```
**Características:**
- ✅ Livre de deadlock
- ✅ Determinístico e simples
- ❌ Não adaptativo a congestionamentos

### 2. 🌊 West-First Routing (Adaptativo)
```cpp
// Prioridade: WEST primeiro, depois adaptativo
if (dest_x < current_x) → must go WEST
else → choose adaptively among {EAST, NORTH, SOUTH}
```
**Características:**
- ✅ Parcialmente adaptativo
- ✅ Pode evitar congestionamentos
- ❌ Mais complexo computacionalmente

## 🧪 Testes Automatizados

O projeto inclui um sistema completo de testes automatizados que compara o desempenho dos algoritmos de roteamento XY e West-First sob diferentes condições de tráfego.

### Executar Todos os Testes

```bash
# Compilar e executar suite completa de testes
mingw32-make test

# Ou executar diretamente os scripts organizados
.\scripts\run_tests.bat
```

Este comando irá:
- Compilar o projeto automaticamente
- Executar 6 testes diferentes (XY e West-First com taxas de 5%, 10% e 20%)
- Salvar os resultados em `test_outputs/` com timestamp

### Analisar Resultados

```bash
# Analisar resultados com análise estatística
mingw32-make analyze

# Ou executar diretamente o script organizado
python .\scripts\analyze_results_simple.py
```

O analisador gera:
- 📊 **CSV compilado** com todos os dados tabulados
- 📈 **Estatísticas comparativas** entre algoritmos
- 📄 **Relatório formatado** com métricas de performance
- 🔍 **Análise estatística** identificando o melhor algoritmo por cenário

### Estrutura dos Resultados

```
test_outputs/
├── test_XY_5pct_2025-01-13_14-30-15.txt      # XY com 5% injeção
├── test_XY_10pct_2025-01-13_14-30-45.txt     # XY com 10% injeção
├── test_XY_20pct_2025-01-13_14-31-15.txt     # XY com 20% injeção
├── test_WestFirst_5pct_2025-01-13_14-31-45.txt   # West-First com 5% injeção
├── test_WestFirst_10pct_2025-01-13_14-32-15.txt  # West-First com 10% injeção
├── test_WestFirst_20pct_2025-01-13_14-32-45.txt  # West-First com 20% injeção
├── resultados_compilados.csv              # Dados tabulados para análise
├── relatorio_resumo.txt                   # Relatório comparativo
├── latencia_comparacao.png               # Gráfico de latência
├── throughput_comparacao.png             # Gráfico de throughput
└── saltos_comparacao.png                 # Gráfico de saltos médios
```

### Análise Estatística

O sistema utiliza **pandas** para análise avançada:
- Estatísticas descritivas (média, desvio padrão, min/max)
- Comparação entre algoritmos com testes estatísticos
- Identificação automática do melhor algoritmo por métrica
- Visualizações em boxplot para comparação de distribuições

## 🛠️ Compilação e Execução

### ⚙️ Requisitos
- **SystemC** 2.3.3+
- **MinGW-w64** ou GCC com C++17
- **Windows PowerShell** ou terminal compatível

### 🔨 Compilação
```bash
# No diretório do projeto
mingw32-make

# Ou usando scripts de build organizados
.\build.bat        # Script Windows para compilação
.\check_setup.bat  # Verificar ambiente antes de compilar
```

### 🚀 Execução
```bash
# Simulação básica (malha 4x4, XY routing)
.\bin\noc_simulation.exe

# Execução rápida com script
.\run.bat

# Simulação customizada
.\bin\noc_simulation.exe -size 6 -routing WEST_FIRST -rate 20 -time 500

# Ver todas as opções
.\bin\noc_simulation.exe -help
```

### 🎛️ Parâmetros de Simulação

| Parâmetro | Descrição | Valores | Padrão |
|-----------|-----------|---------|--------|
| `-size` | Tamanho da malha | 2-16 | 4 |
| `-routing` | Algoritmo | XY, WEST_FIRST | XY |
| `-rate` | Taxa de injeção (%) | 1-100 | 10 |
| `-time` | Ciclos de simulação | 100+ | 1000 |

## 📊 Resultados e Estatísticas

### 📈 Métricas Coletadas
- **Pacotes enviados/recebidos** por nó
- **Latência média** (tempo de transmissão)
- **Número médio de saltos** (hops)
- **Caminhos percorridos** pelos pacotes

### 📋 Exemplo de Saída
```
Node 10 sending packet to 12 (payload: 684) at time 2
Node 12 received Packet[10->12, Type: DATA, Payload: 684, Hops: 4, 
    Path: 10 -> 9 -> 8 -> 12] at time 10 (Latency: 8, Hops: 4)

-------- Simulation Statistics --------
Node 0: Sent=15, Received=12, Avg Latency=8.5, Avg Hops=3.2
Network Summary: Total Sent=240, Total Received=235, 
    Avg Latency=9.2, Avg Hops=3.8
```

## �️ Organização do Projeto

### 📂 Diretórios Principais

| Diretório | Descrição | Conteúdo |
|-----------|-----------|----------|
| **`src/`** | Código-fonte principal | Implementação SystemC da NoC |
| **`scripts/`** | Scripts de automação | Testes e análises automatizadas |
| **`bin/`** | Executáveis | Simulador compilado |
| **`build/`** | Arquivos de build | Cache e temporários do CMake |
| **`test_outputs/`** | Resultados de teste | Logs, CSVs e relatórios |

### 🛠️ Scripts de Conveniência

| Script | Função | Descrição |
|--------|--------|-----------|
| `build.bat` | Compilação | Compila o projeto com configurações otimizadas |
| `run.bat` | Execução rápida | Executa simulação com parâmetros padrão |
| `check_setup.bat` | Verificação | Valida ambiente SystemC antes da compilação |
| `scripts/run_tests.bat` | Testes automatizados | Suite completa de testes comparativos |
| `scripts/analyze_results_simple.py` | Análise | Processamento estatístico dos resultados |

### 📋 Arquivos de Configuração

- **`Makefile`**: Sistema de build principal para MinGW
- **`CMakeLists.txt`**: Configuração alternativa para CMake
- **`SETUP.md`**: Guia detalhado de configuração do ambiente
- **`test_cpp.cpp`**: Testes unitários para validação

## �📁 Estrutura do Projeto

```
ChipEmRede/
├── 📄 CMakeLists.txt      # Configuração CMake alternativa
├── 📄 Makefile            # Sistema de build principal
├── 📄 README.md           # Esta documentação
├── 📄 SETUP.md            # Guia de configuração do ambiente
├── 📄 trab2.txt           # Especificação do trabalho acadêmico
├── 📄 test_cpp.cpp        # Teste unitário C++
├── 📄 analyze_results.py  # Análise automática de resultados (root)
├── 🔧 build.bat           # Script de compilação Windows
├── � run.bat             # Script de execução rápida
├── 🔧 check_setup.bat     # Verificação do ambiente
├── 📂 src/                # Código-fonte principal
│   ├── 🎯 main.cpp        # Ponto de entrada da simulação
│   ├── 🌐 noc.h           # Implementação da NoC
│   ├── 🎯 node.cpp        # Implementação dos nós (source)
│   ├── 🎯 node.h          # Implementação dos nós (header)
│   ├── 🔀 router.cpp      # Implementação dos roteadores (source)
│   ├── 🔀 router.h        # Implementação dos roteadores (header)
│   ├── 📦 packet.h        # Estrutura de pacotes
│   ├── 📡 channel.h       # Canais de comunicação
│   └── 🧠 routing_algorithms.h # Algoritmos de roteamento
├── 📂 scripts/            # Scripts de automação
│   ├── 🤖 run_tests.bat   # Testes automatizados
│   └── 📊 analyze_results_simple.py # Análise estatística avançada
├── 📂 bin/                # Executáveis compilados
│   └── 🚀 noc_simulation.exe # Simulador principal
├── 📂 build/              # Arquivos de compilação (CMake)
│   ├── CMakeCache.txt     # Cache do CMake
│   └── CMakeFiles/        # Arquivos temporários do CMake
└── 📂 test_outputs/       # Resultados dos testes automatizados
    ├── 📄 README.md       # Documentação dos outputs
    ├── 📊 *.txt           # Arquivos de resultado das simulações
    ├── 📈 *.png           # Gráficos comparativos
    └── 📋 analysis_summary.csv # Resumo estatístico
```

## 🧪 Exemplos de Teste

### 🔬 Teste Manual
```bash
# XY Routing
.\bin\noc_simulation.exe -size 4 -routing XY -rate 15 -time 200

# West-First Routing  
.\bin\noc_simulation.exe -size 4 -routing WEST_FIRST -rate 15 -time 200
```

### 🤖 Testes Automatizados

O projeto inclui um sistema completo de testes automatizados que executa múltiplos cenários e gera relatórios organizados:

```bash
# Executar todos os testes automatizados
mingw32-make test
# ou diretamente:
.\scripts\run_tests.bat

# Analisar resultados (com análise estatística)
mingw32-make analyze
# ou diretamente:
python .\scripts\analyze_results_simple.py
```

#### 📊 Cenários de Teste Incluídos:

1. **Comparação de Algoritmos** - XY vs West-First em condições idênticas
2. **Escalabilidade** - Malhas 3x3, 6x6 e 8x8 com diferentes taxas
3. **Taxa de Injeção** - Impacto de baixa (5%), média (25%) e alta (50%) injeção
4. **Simulações Longas** - 2000 ciclos para estatísticas robustas
5. **Casos Extremos** - Condições de stress para teste de robustez

#### 📁 Outputs Organizados:
```
test_outputs/
├── 01_XY_4x4_rate15_[timestamp].txt
├── 01_WestFirst_4x4_rate15_[timestamp].txt
├── 02_XY_3x3_scale_[timestamp].txt
├── ... (10 arquivos de teste)
├── analysis_summary.csv
├── latency_comparison.png
└── delivery_vs_injection.png
```

### 🔬 Teste Manual Específico
```bash
# Malha pequena
.\bin\noc_simulation.exe -size 3 -rate 25

# Malha grande
.\bin\noc_simulation.exe -size 8 -rate 10 -time 2000
```

### 🔬 Teste 3: Análise de Congestionamento
```bash
# Baixa injeção
.\bin\noc_simulation.exe -rate 5

# Alta injeção
.\bin\noc_simulation.exe -rate 50
```

## 📚 Análise Comparativa

| Aspecto | XY Routing | West-First |
|---------|------------|------------|
| **Complexidade** | Baixa | Média |
| **Adaptabilidade** | Nenhuma | Parcial |
| **Deadlock** | Livre | Livre |
| **Latência** | Fixa | Variável |
| **Throughput** | Limitado | Melhor sob congestionamento |

## 🐛 Resolução de Problemas

### ❌ Erro de Compilação
```bash
# Verificar se SystemC está instalado
echo $SYSTEMC_HOME

# Recompilar
mingw32-make clean
mingw32-make
```

### ❌ Arquivos Indesejados Retornando
```bash
git status           # Ver mudanças
git checkout .       # Restaurar estado limpo
git clean -fd        # Remover arquivos não versionados
```

## 📋 Status de Desenvolvimento

| Requisito | Status | Descrição |
|-----------|--------|-----------|
| ✅ **Código SystemC** | Completo | Todos os componentes implementados |
| ✅ **Dois Algoritmos** | Completo | XY e West-First funcionais |
| ✅ **Simulações** | Completo | Múltiplos cenários testados |
| ❌ **Relatório PDF** | Pendente | Documentação técnica |
| ❌ **Vídeo Demo** | Pendente | Demonstração visual |

## 👥 Trabalho Acadêmico

**Disciplina**: Concepção e Implementação de Rede em Chip  
**Data de Entrega**: 14/07/2025  
**Apresentação**: 16/07/2025  

---

*🎓 Projeto desenvolvido para fins acadêmicos demonstrando implementação de NoC com SystemC*