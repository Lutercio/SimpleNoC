# SimpleNoC - Simulador Network-on-Chip

Um framework completo de simulação Network-on-Chip (NoC) implementado em SystemC com ferramentas de análise e visualização baseadas em Python.

## 🏗️ Estrutura do Projeto

```
SimpleNoC/
├── src/                          # Código fonte SystemC
│   ├── packet.h                  # Estrutura e tipos de pacotes
│   ├── node.h                    # Implementação dos nós da rede
│   ├── router.h                  # Roteador com interface de algoritmos
│   ├── routing_algorithms.h      # Algoritmos XY e West-First
│   ├── channel.h                 # Canais de comunicação
│   ├── noc.h                     # Classe principal do NoC
│   └── main.cpp                  # Ponto de entrada da simulação
├── scripts/                      # Análise e visualização
│   ├── run_tests_50.bat         # Suite automatizada de testes
│   ├── analyze_results.py        # Processamento de dados
│   ├── generate_png_graphs.py    # Visualizações estáticas
│   ├── generate_html_charts.py   # Gráficos interativos
│   └── generate_all_graphs.py    # Script mestre de geração
├── graphs/                       # Visualizações geradas
│   ├── index.html               # Dashboard interativo
│   └── *.png                    # Gráficos de comparação
├── Makefile                     # Automação de build
└── README.md                    # Este arquivo
```

## 🚀 Início Rápido

### Pré-requisitos
- SystemC 2.3.3+ instalado
- MinGW64/MSYS2 (Windows) ou GCC (Linux)
- Python 3.8+ com matplotlib, pandas

### Compilar e Executar
```bash
# Compilar o simulador
make all

# Executar teste único
bin/noc_simulation.exe XY 4 4 10 100

# Executar suite completa de testes
make test-50

# Gerar análise e gráficos
make graphs
```

## 🔍 Funcionalidades

### Algoritmos de Roteamento
- **Roteamento XY**: Roteamento determinístico dimension-ordered
- **West-First**: Algoritmo turn model que previne deadlocks

### Métricas Analisadas
- **Latência**: Tempo médio de entrega dos pacotes
- **Throughput**: Pacotes processados por ciclo de simulação
- **Hops**: Número médio de travessias de roteadores

### Visualização
- **Gráficos PNG Estáticos**: Gráficos de análise comparativa
- **Dashboard HTML Interativo**: Ferramentas de exploração baseadas na web

## 📊 Resultados de Exemplo

O simulador compara a performance dos algoritmos em diferentes:
- Tamanhos de mesh (4x4 a 16x16)
- Taxas de injeção (5% a 40%)
- Padrões de tráfego (uniforme aleatório)

Os resultados mostram trade-offs entre roteamento determinístico (XY) e abordagens adaptativas (West-First).

## 🛠️ Desenvolvimento

### Componentes Principais
- **Packet**: Estrutura de dados para tráfego de rede
- **Node**: Elementos de processamento que geram/consomem pacotes
- **Router**: Elementos de encaminhamento implementando algoritmos de roteamento
- **Channel**: Primitivas de comunicação SystemC
- **NoC**: Instanciação da rede de nível superior

### Pontos de Extensão
- Adicionar novos algoritmos de roteamento em `routing_algorithms.h`
- Implementar diferentes topologias em `noc.h`
- Estender coleta de métricas nos scripts de análise

## 📈 Pipeline de Análise

1. **Simulação**: SystemC gera logs detalhados
2. **Processamento**: Scripts Python fazem parsing e agregação dos dados
3. **Visualização**: Gráficos revelam insights de performance
4. **Comparação**: Avaliação lado a lado dos algoritmos

## 🎯 Casos de Uso
- Pesquisa em arquitetura de computadores
- Desenvolvimento de algoritmos NoC
- Benchmarking de performance
- Demonstrações educacionais
