# Pasta de Resultados dos Testes

Esta pasta contém os resultados dos testes automatizados da simulação NoC.

## Estrutura dos arquivos:

- `test_XY_*pct_*.txt` - Resultados do algoritmo XY com diferentes taxas de injeção
- `test_WestFirst_*pct_*.txt` - Resultados do algoritmo West-First com diferentes taxas
- `resultados_compilados.csv` - Dados tabulados para análise (gerado pelo analisador)
- `relatorio_resumo.txt` - Relatório de análise comparativa (gerado pelo analisador)
- `*.png` - Gráficos comparativos (se matplotlib disponível)

## Como gerar os testes:

```bash
# Executar todos os testes automatizados
mingw32-make test

# Analisar os resultados com pandas
mingw32-make analyze
```

## Formato dos nomes dos arquivos:

`test_{ALGORITMO}_{TAXA}pct_{TIMESTAMP}.txt`

Onde:
- `ALGORITMO`: XY ou WestFirst
- `TAXA`: Taxa de injeção de pacotes (5, 10, 20)
- `TIMESTAMP`: Data e hora da execução

## Análise dos dados:

O script `analyze_results.py` usa pandas para:
- Comparar desempenho entre algoritmos
- Gerar estatísticas descritivas
- Criar gráficos comparativos
- Exportar dados para CSV

Os arquivos gerados aqui são essenciais para o relatório final do projeto.
