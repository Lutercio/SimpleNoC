# Análise de Outputs dos Testes

Esta pasta contém os resultados dos testes automatizados da simulação NoC.

## 📁 Organização dos Arquivos

### Nomenclatura dos Arquivos:
```
[NUMERO]_[ALGORITMO]_[CONFIGURACAO]_[TIMESTAMP].txt
```

### Tipos de Teste:

#### 01 - Comparação de Algoritmos
- `01_XY_4x4_rate15_*.txt` - XY routing em malha 4x4 com 15% de injeção
- `01_WestFirst_4x4_rate15_*.txt` - West-First routing nas mesmas condições

#### 02 - Escalabilidade de Malha
- `02_XY_3x3_scale_*.txt` - Malha 3x3 com alta injeção (20%)
- `02_XY_6x6_scale_*.txt` - Malha 6x6 com injeção moderada (10%)
- `02_XY_8x8_scale_*.txt` - Malha 8x8 com baixa injeção (8%)

#### 03 - Impacto da Taxa de Injeção
- `03_WestFirst_rate05_*.txt` - Taxa baixa (5%)
- `03_WestFirst_rate25_*.txt` - Taxa alta (25%)
- `03_WestFirst_rate50_*.txt` - Taxa muito alta (50%)

#### 04 - Simulações Longas
- `04_XY_long_simulation_*.txt` - XY com 2000 ciclos
- `04_WestFirst_long_simulation_*.txt` - West-First com 2000 ciclos

#### 05 - Casos Extremos
- `05_XY_2x2_extreme_*.txt` - Malha pequena com alta injeção
- `05_WestFirst_5x5_extreme_*.txt` - Malha média com injeção extrema

## 📊 Métricas Analisadas

Para cada teste, analise:

### Desempenho Geral:
- **Total Sent** vs **Total Received** (eficiência)
- **Avg Latency** (tempo médio de entrega)
- **Avg Hops** (número médio de saltos)

### Por Nó:
- Distribuição de pacotes enviados/recebidos
- Variação de latência entre nós
- Identificação de gargalos

## 🔍 Como Analisar

1. **Compare arquivos 01**: Diferenças entre XY e West-First
2. **Analise arquivos 02**: Como performance varia com tamanho da malha
3. **Examine arquivos 03**: Impacto da taxa de injeção no congestionamento
4. **Verifique arquivos 04**: Estatísticas com simulações longas
5. **Investigue arquivos 05**: Comportamento em condições extremas

## 📈 Indicadores-Chave

### Performance Boa:
- Latência baixa (< 10 ciclos)
- Taxa de entrega alta (> 95%)
- Distribuição uniforme entre nós

### Problemas Identificados:
- Latência muito alta (congestionamento)
- Muitos pacotes perdidos
- Desbalanceamento entre nós

---
*Arquivos gerados automaticamente pelo sistema de testes*
