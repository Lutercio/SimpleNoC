#!/usr/bin/env python3
"""
Gerador de gráficos simples para NoC usando apenas bibliotecas padrão
Cria gráficos em HTML para visualização em navegador
"""

import os
import re
import glob
import json
from datetime import datetime

def parse_test_results():
    """Analisar todos os arquivos de teste"""
    pattern = os.path.join("test_outputs", "test_*.txt")
    files = glob.glob(pattern)
    
    results = []
    
    for filepath in files:
        filename = os.path.basename(filepath)
        
        # Extrair informações do nome do arquivo
        pattern = r'test_(\w+)_(?:(\d+)x\d+_)?(\d+)pct_(.+)\.txt'
        match = re.match(pattern, filename)
        
        if not match:
            continue
            
        algorithm = match.group(1)
        mesh_size = int(match.group(2)) if match.group(2) else 4
        injection_rate = int(match.group(3))
        timestamp = match.group(4)
        
        # Ler arquivo e extrair estatísticas
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Procurar estatísticas de rede
            stats_pattern = r'Network Summary: Total Sent=(\d+), Total Received=(\d+), Avg Latency=([0-9.]+), Avg Hops=([0-9.]+)'
            stats_match = re.search(stats_pattern, content)
            
            if stats_match:
                total_sent = int(stats_match.group(1))
                total_received = int(stats_match.group(2))
                avg_latency = float(stats_match.group(3))
                avg_hops = float(stats_match.group(4))
                throughput = (total_received / total_sent * 100) if total_sent > 0 else 0
                
                results.append({
                    'algorithm': algorithm,
                    'mesh_size': mesh_size,
                    'injection_rate': injection_rate,
                    'total_sent': total_sent,
                    'total_received': total_received,
                    'avg_latency': avg_latency,
                    'avg_hops': avg_hops,
                    'throughput': throughput,
                    'timestamp': timestamp
                })
        
        except Exception as e:
            print(f"Erro ao processar {filename}: {e}")
    
    return results

def generate_html_chart(data, title, x_label, y_label, output_file):
    """Gerar gráfico HTML usando Chart.js"""
    
    # Separar dados por algoritmo
    algorithms = {}
    for item in data:
        alg = item['algorithm']
        if alg not in algorithms:
            algorithms[alg] = {'x': [], 'y': []}
        algorithms[alg]['x'].append(item['x'])
        algorithms[alg]['y'].append(item['y'])
    
    # Cores para diferentes algoritmos
    colors = {
        'XY': '#FF6384',
        'WestFirst': '#36A2EB',
        'WEST_FIRST': '#36A2EB'
    }
    
    html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <title>{title}</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        .chart-container {{ width: 80%; margin: 0 auto; }}
        h1 {{ text-align: center; color: #333; }}
        .info {{ text-align: center; color: #666; margin-bottom: 20px; }}
    </style>
</head>
<body>
    <h1>{title}</h1>
    <div class="info">Gerado em: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</div>
    <div class="chart-container">
        <canvas id="myChart"></canvas>
    </div>
    
    <script>
        const ctx = document.getElementById('myChart').getContext('2d');
        const chart = new Chart(ctx, {{
            type: 'line',
            data: {{
                datasets: ["""
    
    datasets = []
    for i, (alg, alg_data) in enumerate(algorithms.items()):
        color = colors.get(alg, '#FFCE56')
        
        # Ordenar dados por x
        combined = list(zip(alg_data['x'], alg_data['y']))
        combined.sort(key=lambda item: item[0])
        x_sorted, y_sorted = zip(*combined) if combined else ([], [])
        
        dataset = f"""
                    {{
                        label: '{alg}',
                        data: {[{'x': x, 'y': y} for x, y in zip(x_sorted, y_sorted)]},
                        borderColor: '{color}',
                        backgroundColor: '{color}20',
                        fill: false,
                        tension: 0.1
                    }}"""
        datasets.append(dataset)
    
    html_content += ','.join(datasets)
    
    html_content += f"""
                ]
            }},
            options: {{
                responsive: true,
                plugins: {{
                    title: {{
                        display: true,
                        text: '{title}'
                    }},
                    legend: {{
                        display: true,
                        position: 'top'
                    }}
                }},
                scales: {{
                    x: {{
                        display: true,
                        title: {{
                            display: true,
                            text: '{x_label}'
                        }}
                    }},
                    y: {{
                        display: true,
                        title: {{
                            display: true,
                            text: '{y_label}'
                        }}
                    }}
                }}
            }}
        }});
    </script>
</body>
</html>"""
    
    os.makedirs('graphs', exist_ok=True)
    with open(f'graphs/{output_file}', 'w', encoding='utf-8') as f:
        f.write(html_content)
    
    print(f"Gráfico salvo: graphs/{output_file}")

def generate_all_charts():
    """Gerar todos os gráficos"""
    results = parse_test_results()
    
    if not results:
        print("Nenhum resultado encontrado!")
        return
    
    print(f"Processando {len(results)} resultados...")
    
    # Gráfico 1: Latência vs Taxa de Injeção
    latency_data = []
    for r in results:
        latency_data.append({
            'algorithm': r['algorithm'],
            'x': r['injection_rate'],
            'y': r['avg_latency']
        })
    
    generate_html_chart(
        latency_data,
        'Latência vs Taxa de Injeção',
        'Taxa de Injeção (%)',
        'Latência Média (ciclos)',
        'latency_vs_injection.html'
    )
    
    # Gráfico 2: Throughput vs Taxa de Injeção
    throughput_data = []
    for r in results:
        throughput_data.append({
            'algorithm': r['algorithm'],
            'x': r['injection_rate'],
            'y': r['throughput']
        })
    
    generate_html_chart(
        throughput_data,
        'Throughput vs Taxa de Injeção',
        'Taxa de Injeção (%)',
        'Throughput (%)',
        'throughput_vs_injection.html'
    )
    
    # Gráfico 3: Saltos vs Taxa de Injeção
    hops_data = []
    for r in results:
        hops_data.append({
            'algorithm': r['algorithm'],
            'x': r['injection_rate'],
            'y': r['avg_hops']
        })
    
    generate_html_chart(
        hops_data,
        'Número de Saltos vs Taxa de Injeção',
        'Taxa de Injeção (%)',
        'Número Médio de Saltos',
        'hops_vs_injection.html'
    )
    
    # Gerar índice HTML
    generate_index_html()
    
    print("\nTodos os gráficos foram gerados!")
    print("Abra 'graphs/index.html' no seu navegador para visualizar")

def generate_index_html():
    """Gerar página índice com links para todos os gráficos"""
    
    html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Análise NoC - Gráficos</title>
    <style>
        body {{ 
            font-family: Arial, sans-serif; 
            margin: 40px; 
            background-color: #f5f5f5;
        }}
        .container {{ 
            max-width: 800px; 
            margin: 0 auto; 
            background: white; 
            padding: 30px; 
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        }}
        h1 {{ 
            color: #333; 
            text-align: center; 
            margin-bottom: 30px;
        }}
        .chart-link {{ 
            display: block; 
            padding: 15px; 
            margin: 10px 0; 
            background: #007bff; 
            color: white; 
            text-decoration: none; 
            border-radius: 5px; 
            text-align: center;
            transition: background 0.3s;
        }}
        .chart-link:hover {{ 
            background: #0056b3; 
        }}
        .info {{ 
            background: #e9ecef; 
            padding: 15px; 
            border-radius: 5px; 
            margin-bottom: 20px;
        }}
        .footer {{ 
            text-align: center; 
            margin-top: 30px; 
            color: #666; 
            font-size: 12px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>📊 Análise NoC - Gráficos Interativos</h1>
        
        <div class="info">
            <strong>Sistema de Análise SimpleNoC</strong><br>
            Gráficos gerados em: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}<br>
            Clique nos links abaixo para visualizar os gráficos interativos.
        </div>
        
        <a href="latency_vs_injection.html" class="chart-link">
            📈 Latência vs Taxa de Injeção
        </a>
        
        <a href="throughput_vs_injection.html" class="chart-link">
            📊 Throughput vs Taxa de Injeção
        </a>
        
        <a href="hops_vs_injection.html" class="chart-link">
            🔀 Número de Saltos vs Taxa de Injeção
        </a>
        
        <div class="footer">
            Sistema de análise SimpleNoC - Network-on-Chip Simulation
        </div>
    </div>
</body>
</html>"""
    
    with open('graphs/index.html', 'w', encoding='utf-8') as f:
        f.write(html_content)
    
    print("Índice salvo: graphs/index.html")

if __name__ == "__main__":
    print("="*60)
    print("GERADOR DE GRÁFICOS HTML - NoC ANALYSIS")
    print("="*60)
    print("Gerando gráficos interativos em HTML...")
    print()
    
    generate_all_charts()
