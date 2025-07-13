#!/usr/bin/env python3
"""
Analisador de Resultados dos Testes NoC
=========================================================

"""

import os
import sys
import glob
import re
from datetime import datetime

def parse_output_file(filepath):
    """Extrai dados de um arquivo de saída da simulação"""
    data = {
        'timestamp': None,
        'routing_algorithm': None,
        'mesh_size': None,
        'injection_rate': None,
        'simulation_time': None,
        'total_sent': 0,
        'total_received': 0,
        'network_avg_latency': 0.0,
        'network_avg_hops': 0.0,
        'throughput': 0.0,
        'network_efficiency': 1.0,
        'nodes': []
    }
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            
        # Extrair informações do cabeçalho
        lines = content.split('\n')
        for line in lines:
            if 'Tamanho da malha:' in line:
                match = re.search(r'(\d+)x(\d+)', line)
                if match:
                    data['mesh_size'] = f"{match.group(1)}x{match.group(2)}"
            elif 'Algoritmo de roteamento:' in line:
                data['routing_algorithm'] = line.split(':')[-1].strip()
            elif 'Taxa de injeção de pacotes:' in line:
                match = re.search(r'(\d+)%', line)
                if match:
                    data['injection_rate'] = int(match.group(1))
            elif 'Tempo de simulação:' in line:
                match = re.search(r'(\d+)', line)
                if match:
                    data['simulation_time'] = int(match.group(1))
            elif line.startswith('Node '):
                # Parsear dados do nó
                match = re.search(r'Node (\d+): Sent=(\d+), Received=(\d+), Avg Latency=([0-9.]+), Avg Hops=([0-9.]+)', line)
                if match:
                    node_data = {
                        'id': int(match.group(1)),
                        'sent': int(match.group(2)),
                        'received': int(match.group(3)),
                        'avg_latency': float(match.group(4)),
                        'avg_hops': float(match.group(5))
                    }
                    data['nodes'].append(node_data)
            elif 'Network Summary:' in line:
                # Parsear resumo da rede
                match = re.search(r'Total Sent=(\d+), Total Received=(\d+), Avg Latency=([0-9.]+), Avg Hops=([0-9.]+)', line)
                if match:
                    data['total_sent'] = int(match.group(1))
                    data['total_received'] = int(match.group(2))
                    data['network_avg_latency'] = float(match.group(3))
                    data['network_avg_hops'] = float(match.group(4))
        
        # Calcular métricas adicionais
        if data['total_sent'] > 0:
            data['throughput'] = data['total_received'] / data['simulation_time'] if data['simulation_time'] > 0 else 0
            # Nota: total_received conta todos os hops, não pacotes únicos entregues
            data['network_efficiency'] = data['total_sent'] / (data['total_received'] / data['network_avg_hops']) if data['total_received'] > 0 else 0
        
        # Extrair timestamp do nome do arquivo
        filename = os.path.basename(filepath)
        timestamp_match = re.search(r'(\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2})', filename)
        if timestamp_match:
            data['timestamp'] = timestamp_match.group(1)
            
    except Exception as e:
        print(f"Erro ao analisar {filepath}: {e}")
        
    return data

def generate_csv_export(results, output_dir):
    """Exporta dados para CSV"""
    csv_path = os.path.join(output_dir, 'resultados_compilados.csv')
    
    with open(csv_path, 'w', encoding='utf-8', newline='') as f:
        # Cabeçalho
        f.write("timestamp,routing_algorithm,mesh_size,injection_rate,simulation_time,")
        f.write("total_sent,total_received,network_avg_latency,network_avg_hops,throughput,network_efficiency\n")
        
        # Dados
        for result in results:
            f.write(f"{result['timestamp']},{result['routing_algorithm']},{result['mesh_size']},")
            f.write(f"{result['injection_rate']},{result['simulation_time']},")
            f.write(f"{result['total_sent']},{result['total_received']},")
            f.write(f"{result['network_avg_latency']},{result['network_avg_hops']},")
            f.write(f"{result['throughput']},{result['network_efficiency']}\n")
    
    print(f"✓ Dados exportados para CSV: {csv_path}")

def calculate_stats(values):
    """Calcula estatísticas básicas de uma lista de valores"""
    if not values:
        return {'mean': 0, 'std': 0, 'min': 0, 'max': 0}
    
    mean = sum(values) / len(values)
    variance = sum((x - mean) ** 2 for x in values) / len(values)
    std = variance ** 0.5
    
    return {
        'mean': mean,
        'std': std,
        'min': min(values),
        'max': max(values)
    }

def generate_summary_report(results, output_dir):
    """Gera relatório de resumo dos resultados"""
    report_path = os.path.join(output_dir, 'relatorio_resumo.txt')
    
    with open(report_path, 'w', encoding='utf-8') as f:
        f.write("RELATÓRIO DE ANÁLISE DOS TESTES NOC\n")
        f.write("="*50 + "\n\n")
        f.write(f"Data da análise: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
        f.write(f"Total de testes analisados: {len(results)}\n\n")
        
        # Agrupar por algoritmo
        algorithms = {}
        for result in results:
            alg = result['routing_algorithm']
            if alg not in algorithms:
                algorithms[alg] = []
            algorithms[alg].append(result)
        
        # Estatísticas por algoritmo
        f.write("ESTATÍSTICAS POR ALGORITMO\n")
        f.write("-" * 30 + "\n")
        
        for alg_name, alg_results in algorithms.items():
            f.write(f"\n{alg_name.upper()}\n")
            f.write("." * 20 + "\n")
            f.write(f"Número de testes: {len(alg_results)}\n")
            
            if alg_results:
                latencies = [r['network_avg_latency'] for r in alg_results]
                hops = [r['network_avg_hops'] for r in alg_results]
                throughputs = [r['throughput'] for r in alg_results]
                
                lat_stats = calculate_stats(latencies)
                hop_stats = calculate_stats(hops)
                thru_stats = calculate_stats(throughputs)
                
                f.write(f"Latência média: {lat_stats['mean']:.2f} ± {lat_stats['std']:.2f} ciclos\n")
                f.write(f"Saltos médios: {hop_stats['mean']:.2f} ± {hop_stats['std']:.2f}\n")
                f.write(f"Throughput médio: {thru_stats['mean']:.3f} ± {thru_stats['std']:.3f} pacotes/ciclo\n")
        
        # Comparação de desempenho
        if len(algorithms) >= 2:
            f.write(f"\n\nCOMPARAÇÃO DE DESEMPENHO\n")
            f.write("="*30 + "\n")
            
            best_latency_alg = None
            best_latency_value = float('inf')
            best_throughput_alg = None
            best_throughput_value = 0
            
            for alg_name, alg_results in algorithms.items():
                if alg_results:
                    avg_latency = sum(r['network_avg_latency'] for r in alg_results) / len(alg_results)
                    avg_throughput = sum(r['throughput'] for r in alg_results) / len(alg_results)
                    
                    if avg_latency < best_latency_value:
                        best_latency_value = avg_latency
                        best_latency_alg = alg_name
                    
                    if avg_throughput > best_throughput_value:
                        best_throughput_value = avg_throughput
                        best_throughput_alg = alg_name
            
            f.write(f"✓ Menor latência média: {best_latency_alg} ({best_latency_value:.2f} ciclos)\n")
            f.write(f"✓ Maior throughput médio: {best_throughput_alg} ({best_throughput_value:.3f} pacotes/ciclo)\n")
        
        f.write(f"\nRelatório salvo em: {report_path}\n")
    
    print(f"✓ Relatório de resumo salvo em: {report_path}")

def main():
    """Função principal"""
    print("=== Analisador de Resultados NoC (Versão Simples) ===\n")
    
    # Verificar se a pasta de outputs existe
    output_dir = 'test_outputs'
    if not os.path.exists(output_dir):
        print(f"❌ Pasta {output_dir} não encontrada!")
        print("Execute primeiro: mingw32-make test")
        return 1
    
    # Encontrar todos os arquivos de saída
    pattern = os.path.join(output_dir, "test_*.txt")  # Só os novos arquivos de teste
    output_files = glob.glob(pattern)
    
    if not output_files:
        print(f"❌ Nenhum arquivo de teste encontrado em {output_dir}/")
        print("Execute primeiro: mingw32-make test")
        return 1
    
    print(f"📁 Encontrados {len(output_files)} arquivos de teste")
    
    # Analisar cada arquivo
    results = []
    for filepath in output_files:
        print(f"📊 Analisando: {os.path.basename(filepath)}")
        data = parse_output_file(filepath)
        if data['routing_algorithm']:  # Só adiciona se conseguiu extrair dados válidos
            results.append(data)
    
    if not results:
        print("❌ Nenhum resultado válido encontrado!")
        return 1
    
    print(f"✓ {len(results)} testes analisados com sucesso\n")
    
    # Gerar análises
    generate_summary_report(results, output_dir)
    generate_csv_export(results, output_dir)
    
    # Mostrar resumo na tela
    print("\n=== Resumo dos Resultados ===")
    algorithms = {}
    for result in results:
        alg = result['routing_algorithm']
        if alg not in algorithms:
            algorithms[alg] = []
        algorithms[alg].append(result)
    
    for alg_name, alg_results in algorithms.items():
        if alg_results:
            avg_latency = sum(r['network_avg_latency'] for r in alg_results) / len(alg_results)
            avg_throughput = sum(r['throughput'] for r in alg_results) / len(alg_results)
            print(f"{alg_name}: Latência={avg_latency:.2f}, Throughput={avg_throughput:.3f}")
    
    print("\n=== Análise Completa ===")
    print(f"📈 Dados processados: {len(results)} testes")
    print(f"📂 Resultados salvos em: {output_dir}/")
    print(f"📊 CSV exportado: resultados_compilados.csv")
    print(f"📄 Relatório: relatorio_resumo.txt")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
