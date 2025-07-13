# 🌐 Network-on-Chip (NoC) Simulation with SystemC

## 📋 Descrição do Projeto

Este projeto implementa uma simulação completa de **Rede em Chip (Network-on-Chip)** usando **SystemC**, desenvolvido como trabalho acadêmico para a disciplina de arquitetura de sistemas digitais. 

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

## 🛠️ Compilação e Execução

### ⚙️ Requisitos
- **SystemC** 2.3.3+
- **MinGW-w64** ou GCC com C++17
- **Windows PowerShell** ou terminal compatível

### 🔨 Compilação
```bash
# No diretório do projeto
mingw32-make
```

### 🚀 Execução
```bash
# Simulação básica (malha 4x4, XY routing)
.\bin\noc_simulation.exe

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

## 📁 Estrutura do Projeto

```
ChipEmRede/
├── 📄 .gitignore          # Arquivos ignorados pelo Git
├── 📄 Makefile            # Sistema de build
├── 📄 README.md           # Esta documentação
├── 📄 USAGE.md            # Instruções de uso
├── 📄 trab2.txt           # Especificação do trabalho
└── 📂 src/                # Código-fonte
    ├── 🎯 main.cpp        # Ponto de entrada da simulação
    ├── 🌐 noc.h           # Implementação da NoC
    ├── 🎯 node.h          # Implementação dos nós
    ├── 🔀 router.h        # Implementação dos roteadores
    ├── 📦 packet.h        # Estrutura de pacotes
    ├── 📡 channel.h       # Canais de comunicação
    └── 🧠 routing_algorithms.h # Algoritmos de roteamento
```

## 🧪 Exemplos de Teste

### 🔬 Teste 1: Comparação de Algoritmos
```bash
# XY Routing
.\bin\noc_simulation.exe -size 4 -routing XY -rate 15 -time 200

# West-First Routing  
.\bin\noc_simulation.exe -size 4 -routing WEST_FIRST -rate 15 -time 200
```

### 🔬 Teste 2: Análise de Escalabilidade
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