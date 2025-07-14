#!/usr/bin/env python3
"""
Script para gerar gráficos PNG com comparações de algoritmos de roteamento NoC
"""

import os
import sys
import re
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
from pathlib import Path

# Configurar matplotlib para não usar interface gráfica
plt.switch_backend('Agg')

class NoCPNGGraphGenerator:
    def __init__(self, test_outputs_dir):
        self.test_outputs_dir = Path(test_outputs_dir)
        self.results = {}
        
        # Cores distintas para cada algoritmo
        self.colors = {
            'XY': '#1f77b4',      # Azul
            'WestFirst': '#ff7f0e' # Laranja
        }
        
        # Markers distintos
        self.markers = {
            'XY': 'o',        # Círculo
            'WestFirst': 's'  # Quadrado
        }
        
        # Configurar estilo dos gráficos
        plt.style.use('seaborn-v0_8')
        plt.rcParams['figure.figsize'] = (12, 8)
        plt.rcParams['font.size'] = 12
        plt.rcParams['axes.titlesize'] = 14
        plt.rcParams['axes.labelsize'] = 12
        plt.rcParams['legend.fontsize'] = 11
        
    def parse_test_file(self, filepath):
        """Parse um arquivo de teste e extrai métricas"""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            # Extrair informações do nome do arquivo
            filename = filepath.name
            
            # Tentar dois formatos: test_ALGO_MESHxMESH_RANGEpct_* e test_ALGO_RANGEpct_*
            match = re.match(r'test_(\w+)_(\d+)x(\d+)_(\d+)pct_.*\.txt', filename)
            if match:
                algorithm = match.group(1)
                mesh_size = f"{match.group(2)}x{match.group(3)}"
                injection_rate = int(match.group(4))
            else:
                match = re.match(r'test_(\w+)_(\d+)pct_.*\.txt', filename)
                if not match:
                    return None
                algorithm = match.group(1)
                injection_rate = int(match.group(2))
                
                # Para arquivos sem mesh no nome, extrair do conteúdo
                mesh_match = re.search(r'Tamanho da malha:\s*(\d+)x(\d+)', content)
                if not mesh_match:
                    mesh_match = re.search(r'Mesh size:\s*(\d+)x(\d+)', content)
                if not mesh_match:
                    print(f"Não foi possível extrair tamanho da mesh de {filename}")
                    return None
                mesh_size = f"{mesh_match.group(1)}x{mesh_match.group(2)}"
            
            # Extrair métricas do resumo final
            # Buscar por "Network Summary" ou "Avg Latency" no conteúdo
            latency_match = re.search(r'Avg Latency=([\d.]+)', content)
            if not latency_match:
                latency_match = re.search(r'Average latency:\s*([\d.]+)', content)
            
            hops_match = re.search(r'Avg Hops=([\d.]+)', content)
            if not hops_match:
                hops_match = re.search(r'Average hops:\s*([\d.]+)', content)
            
            # Para throughput, calcular como pacotes recebidos / tempo de simulação
            total_received_match = re.search(r'Total Received=(\d+)', content)
            simulation_time_match = re.search(r'Tempo de simulação:\s*(\d+)\s*ciclos', content)
            
            if not all([latency_match, hops_match, total_received_match, simulation_time_match]):
                print(f"Métricas não encontradas em {filename}")
                return None
            
            throughput = float(total_received_match.group(1)) / float(simulation_time_match.group(1))
            
            return {
                'algorithm': algorithm,
                'injection_rate': injection_rate,
                'mesh_size': mesh_size,
                'latency': float(latency_match.group(1)),
                'throughput': throughput,
                'hops': float(hops_match.group(1))
            }
        except Exception as e:
            print(f"Erro ao processar {filepath}: {e}")
            return None
    
    def load_all_results(self):
        """Carrega todos os resultados dos testes"""
        print("Carregando resultados dos testes...")
        
        for test_file in self.test_outputs_dir.glob('test_*.txt'):
            result = self.parse_test_file(test_file)
            if result:
                mesh_size = result['mesh_size']
                algorithm = result['algorithm']
                
                if mesh_size not in self.results:
                    self.results[mesh_size] = {}
                if algorithm not in self.results[mesh_size]:
                    self.results[mesh_size][algorithm] = []
                
                self.results[mesh_size][algorithm].append(result)
        
        # Ordenar por injection rate
        for mesh_size in self.results:
            for algorithm in self.results[mesh_size]:
                self.results[mesh_size][algorithm].sort(key=lambda x: x['injection_rate'])
        
        print(f"Carregados dados para {len(self.results)} tamanhos de mesh")
    
    def create_latency_comparison(self, output_dir):
        """Cria gráfico de comparação de latência por tamanho de mesh"""
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        fig.suptitle('Comparação de Latência por Algoritmo de Roteamento', fontsize=16, fontweight='bold')
        
        mesh_sizes = sorted(self.results.keys())
        axes_flat = axes.flatten()
        
        for idx, mesh_size in enumerate(mesh_sizes[:4]):
            ax = axes_flat[idx]
            
            for algorithm in ['XY', 'WestFirst']:
                if algorithm in self.results[mesh_size]:
                    data = self.results[mesh_size][algorithm]
                    injection_rates = [d['injection_rate'] for d in data]
                    latencies = [d['latency'] for d in data]
                    
                    ax.plot(injection_rates, latencies, 
                           color=self.colors[algorithm],
                           marker=self.markers[algorithm],
                           linewidth=2.5,
                           markersize=8,
                           label=f'{algorithm}',
                           markeredgewidth=1,
                           markeredgecolor='white')
            
            ax.set_title(f'Mesh {mesh_size}', fontweight='bold')
            ax.set_xlabel('Taxa de Injeção (%)')
            ax.set_ylabel('Latência Média (ciclos)')
            ax.grid(True, alpha=0.3)
            ax.legend()
            ax.set_xlim(0, 45)
        
        plt.tight_layout()
        
        output_file = Path(output_dir) / 'latency_comparison.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"Gráfico de latência salvo: {output_file}")
    
    def create_throughput_comparison(self, output_dir):
        """Cria gráfico de comparação de throughput por tamanho de mesh"""
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        fig.suptitle('Comparação de Throughput por Algoritmo de Roteamento', fontsize=16, fontweight='bold')
        
        mesh_sizes = sorted(self.results.keys())
        axes_flat = axes.flatten()
        
        for idx, mesh_size in enumerate(mesh_sizes[:4]):
            ax = axes_flat[idx]
            
            for algorithm in ['XY', 'WestFirst']:
                if algorithm in self.results[mesh_size]:
                    data = self.results[mesh_size][algorithm]
                    injection_rates = [d['injection_rate'] for d in data]
                    throughputs = [d['throughput'] for d in data]
                    
                    ax.plot(injection_rates, throughputs, 
                           color=self.colors[algorithm],
                           marker=self.markers[algorithm],
                           linewidth=2.5,
                           markersize=8,
                           label=f'{algorithm}',
                           markeredgewidth=1,
                           markeredgecolor='white')
            
            ax.set_title(f'Mesh {mesh_size}', fontweight='bold')
            ax.set_xlabel('Taxa de Injeção (%)')
            ax.set_ylabel('Throughput (pacotes/ciclo)')
            ax.grid(True, alpha=0.3)
            ax.legend()
            ax.set_xlim(0, 45)
        
        plt.tight_layout()
        
        output_file = Path(output_dir) / 'throughput_comparison.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"Gráfico de throughput salvo: {output_file}")
    
    def create_hops_comparison(self, output_dir):
        """Cria gráfico de comparação de hops por tamanho de mesh"""
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        fig.suptitle('Comparação de Hops Médios por Algoritmo de Roteamento', fontsize=16, fontweight='bold')
        
        mesh_sizes = sorted(self.results.keys())
        axes_flat = axes.flatten()
        
        for idx, mesh_size in enumerate(mesh_sizes[:4]):
            ax = axes_flat[idx]
            
            for algorithm in ['XY', 'WestFirst']:
                if algorithm in self.results[mesh_size]:
                    data = self.results[mesh_size][algorithm]
                    injection_rates = [d['injection_rate'] for d in data]
                    hops = [d['hops'] for d in data]
                    
                    ax.plot(injection_rates, hops, 
                           color=self.colors[algorithm],
                           marker=self.markers[algorithm],
                           linewidth=2.5,
                           markersize=8,
                           label=f'{algorithm}',
                           markeredgewidth=1,
                           markeredgecolor='white')
            
            ax.set_title(f'Mesh {mesh_size}', fontweight='bold')
            ax.set_xlabel('Taxa de Injeção (%)')
            ax.set_ylabel('Hops Médios')
            ax.grid(True, alpha=0.3)
            ax.legend()
            ax.set_xlim(0, 45)
        
        plt.tight_layout()
        
        output_file = Path(output_dir) / 'hops_comparison.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"Gráfico de hops salvo: {output_file}")
    
    def create_performance_summary(self, output_dir):
        """Cria gráfico resumo de performance"""
        fig, axes = plt.subplots(1, 3, figsize=(20, 6))
        fig.suptitle('Resumo de Performance - Comparação de Algoritmos', fontsize=16, fontweight='bold')
        
        # Calcular médias por mesh size
        mesh_performance = {}
        for mesh_size in self.results:
            mesh_performance[mesh_size] = {}
            for algorithm in self.results[mesh_size]:
                data = self.results[mesh_size][algorithm]
                mesh_performance[mesh_size][algorithm] = {
                    'avg_latency': np.mean([d['latency'] for d in data]),
                    'avg_throughput': np.mean([d['throughput'] for d in data]),
                    'avg_hops': np.mean([d['hops'] for d in data])
                }
        
        mesh_sizes = sorted(mesh_performance.keys())
        algorithms = ['XY', 'WestFirst']
        
        x = np.arange(len(mesh_sizes))
        width = 0.35
        
        # Gráfico de Latência Média
        ax1 = axes[0]
        for i, algorithm in enumerate(algorithms):
            latencies = [mesh_performance[mesh][algorithm]['avg_latency'] for mesh in mesh_sizes]
            ax1.bar(x + i*width, latencies, width, 
                   label=algorithm, color=self.colors[algorithm], alpha=0.8)
        
        ax1.set_title('Latência Média por Mesh', fontweight='bold')
        ax1.set_xlabel('Tamanho da Mesh')
        ax1.set_ylabel('Latência Média (ciclos)')
        ax1.set_xticks(x + width/2)
        ax1.set_xticklabels(mesh_sizes)
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Gráfico de Throughput Médio
        ax2 = axes[1]
        for i, algorithm in enumerate(algorithms):
            throughputs = [mesh_performance[mesh][algorithm]['avg_throughput'] for mesh in mesh_sizes]
            ax2.bar(x + i*width, throughputs, width, 
                   label=algorithm, color=self.colors[algorithm], alpha=0.8)
        
        ax2.set_title('Throughput Médio por Mesh', fontweight='bold')
        ax2.set_xlabel('Tamanho da Mesh')
        ax2.set_ylabel('Throughput Médio (pacotes/ciclo)')
        ax2.set_xticks(x + width/2)
        ax2.set_xticklabels(mesh_sizes)
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        # Gráfico de Hops Médios
        ax3 = axes[2]
        for i, algorithm in enumerate(algorithms):
            hops = [mesh_performance[mesh][algorithm]['avg_hops'] for mesh in mesh_sizes]
            ax3.bar(x + i*width, hops, width, 
                   label=algorithm, color=self.colors[algorithm], alpha=0.8)
        
        ax3.set_title('Hops Médios por Mesh', fontweight='bold')
        ax3.set_xlabel('Tamanho da Mesh')
        ax3.set_ylabel('Hops Médios')
        ax3.set_xticks(x + width/2)
        ax3.set_xticklabels(mesh_sizes)
        ax3.legend()
        ax3.grid(True, alpha=0.3)
        
        plt.tight_layout()
        
        output_file = Path(output_dir) / 'performance_summary.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"Gráfico resumo salvo: {output_file}")
    
    def generate_all_graphs(self, output_dir='graphs'):
        """Gera todos os gráficos PNG"""
        output_path = Path(output_dir)
        output_path.mkdir(exist_ok=True)
        
        print(f"Gerando gráficos PNG em: {output_path.absolute()}")
        
        # Carregar dados
        self.load_all_results()
        
        if not self.results:
            print("Nenhum resultado encontrado!")
            return
        
        # Gerar gráficos
        self.create_latency_comparison(output_path)
        self.create_throughput_comparison(output_path)
        self.create_hops_comparison(output_path)
        self.create_performance_summary(output_path)
        
        print(f"\nTodos os gráficos foram gerados com sucesso!")
        print(f"Diretório: {output_path.absolute()}")
        print("Gráficos criados:")
        print("   - latency_comparison.png")
        print("   - throughput_comparison.png")
        print("   - hops_comparison.png")
        print("   - performance_summary.png")

def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    test_outputs_dir = project_root / 'test_outputs'
    graphs_dir = project_root / 'graphs'
    
    if not test_outputs_dir.exists():
        print(f"❌ Diretório de testes não encontrado: {test_outputs_dir}")
        return 1
    
    generator = NoCPNGGraphGenerator(test_outputs_dir)
    generator.generate_all_graphs(graphs_dir)
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
