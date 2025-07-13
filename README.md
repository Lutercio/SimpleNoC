# Network-on-Chip (NoC) Simulation with Routing Algorithms

Este projeto implementa uma simulação de Rede em Chip (Network-on-Chip ou NoC) usando SystemC, com dois algoritmos de roteamento diferentes: XY (Dimension-Ordered Routing) e West-First.

## Descrição

O projeto simula uma rede em chip com topologia de malha 2D (mesh), onde pacotes são enviados entre diferentes nós da rede. Os pacotes são roteados usando algoritmos de roteamento que podem ser selecionados na execução.

### Componentes Principais

- **Nó (Node)**: Gera e consome pacotes
- **Roteador (Router)**: Encaminha pacotes usando algoritmos de roteamento
- **Canal (Channel)**: Transmite pacotes entre nós e roteadores
- **Pacote (Packet)**: Contém informações como origem, destino e dados
- **Algoritmos de Roteamento**:
  - **XY (Dimension-Ordered)**: Roteia primeiro no eixo X, depois no Y
  - **West-First**: Adaptativo com restrição (sempre oeste primeiro)

## Requisitos

- SystemC (versão 2.3.3 ou superior)
- Compilador C++ com suporte a C++11
- Make

## Compilação

Para compilar o projeto, defina a variável de ambiente `SYSTEMC_HOME` apontando para a instalação do SystemC:

```bash
export SYSTEMC_HOME=/caminho/para/systemc
make
```

## Execução

```bash
./bin/noc_simulation [opções]
```

### Opções

- `-size SIZE`: Define o tamanho da malha (padrão: 4)
- `-routing ALGO`: Define o algoritmo de roteamento: XY, WEST_FIRST (padrão: XY)
- `-rate RATE`: Define a taxa de injeção de pacotes em porcentagem (padrão: 10)
- `-time TIME`: Define o tempo de simulação em ciclos (padrão: 1000)
- `-help, -h`: Exibe ajuda

## Exemplos

### Exemplo 1: Simulação padrão 4x4 com algoritmo XY

```bash
./bin/noc_simulation
```

### Exemplo 2: Malha 8x8 com algoritmo West-First

```bash
./bin/noc_simulation -size 8 -routing WEST_FIRST -rate 15 -time 2000
```

## Estrutura do Projeto

- `src/`: Código-fonte
  - `main.cpp`: Ponto de entrada da simulação
  - `noc.h`: Implementação da rede em chip
  - `node.h/cpp`: Implementação dos nós
  - `router.h/cpp`: Implementação dos roteadores
  - `channel.h`: Implementação dos canais de comunicação
  - `packet.h`: Definição da estrutura de pacotes
  - `routing_algorithms.h`: Implementação dos algoritmos de roteamento
- `bin/`: Arquivos binários compilados
- `build/`: Arquivos de objeto intermediários

## Análise Comparativa dos Algoritmos

### XY Routing (Dimension-Ordered)
- **Prós**: Simples, livre de deadlock, determinístico
- **Contras**: Não adaptativo, pode causar congestionamento

### West-First Routing
- **Prós**: Parcialmente adaptativo, pode evitar congestionamentos
- **Contras**: Mais complexo, potencialmente menos eficiente em certos padrões de tráfego

## Resultados da Simulação

A simulação produz estatísticas para cada nó e resumo da rede:
- Pacotes enviados e recebidos
- Latência média por pacote
- Número médio de saltos (hops) por pacote

Os resultados podem ser comparados entre os dois algoritmos para avaliar desempenho em diferentes cenários de tráfego.