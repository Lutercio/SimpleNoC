#!/usr/bin/env python3
"""
Script mestre para gerar todos os gráficos de análise NoC
"""

import subprocess
import sys
from pathlib import Path

def run_script(script_path, description):
    """Executa um script Python e reporta o resultado"""
    print(f"\n🔄 {description}...")
    try:
        result = subprocess.run([sys.executable, script_path], 
                              capture_output=True, text=True, cwd=script_path.parent.parent)
        if result.returncode == 0:
            print(f"✅ {description} - Concluído com sucesso!")
            if result.stdout:
                print(result.stdout)
        else:
            print(f"❌ {description} - Erro:")
            print(result.stderr)
            return False
    except Exception as e:
        print(f"❌ {description} - Exceção: {e}")
        return False
    return True

def clean_old_files(graphs_dir):
    """Remove arquivos obsoletos antes de gerar novos"""
    clean_script = Path(__file__).parent / "clean_graphs.py"
    if clean_script.exists():
        print("\n🧹 Limpando arquivos obsoletos...")
        try:
            result = subprocess.run([sys.executable, clean_script], 
                                  capture_output=True, text=True, cwd=clean_script.parent.parent)
            if result.returncode == 0:
                print("✅ Limpeza concluída!")
            else:
                print("⚠️  Erro na limpeza, continuando...")
        except Exception:
            print("⚠️  Erro na limpeza, continuando...")

def main():
    """Executa todos os scripts de geração de gráficos"""
    print("🚀 Iniciando geração completa de gráficos NoC...")
    
    script_dir = Path(__file__).parent
    graphs_dir = script_dir.parent / "graphs"
    
    # Limpar arquivos obsoletos primeiro
    clean_old_files(graphs_dir)
    
    scripts = [
        (script_dir / "generate_png_graphs.py", "Gerando gráficos PNG principais"),
        (script_dir / "generate_direct_comparison.py", "Gerando comparações diretas"),
        (script_dir / "generate_html_charts.py", "Gerando gráficos HTML interativos")
    ]
    
    success_count = 0
    total_scripts = len(scripts)
    
    for script_path, description in scripts:
        if script_path.exists():
            if run_script(script_path, description):
                success_count += 1
        else:
            print(f"⚠️  Script não encontrado: {script_path}")
    
    print(f"\n📊 Resumo da Geração:")
    print(f"✅ {success_count}/{total_scripts} scripts executados com sucesso")
    
    if graphs_dir.exists():
        png_files = list(graphs_dir.glob("*.png"))
        html_files = list(graphs_dir.glob("*.html"))
        
        print(f"\n📁 Arquivos gerados em {graphs_dir}:")
        print(f"   🖼️  {len(png_files)} arquivos PNG")
        print(f"   🌐 {len(html_files)} arquivos HTML")
        
        print(f"\n🎯 Principais gráficos disponíveis:")
        key_files = [
            "latency_comparison.png",
            "throughput_comparison.png", 
            "hops_comparison.png",
            "performance_summary.png",
            "direct_algorithm_comparison.png",
            "performance_radar.png",
            "index.html"
        ]
        
        for file in key_files:
            file_path = graphs_dir / file
            if file_path.exists():
                print(f"   ✅ {file}")
            else:
                print(f"   ❌ {file} (não encontrado)")
    
    print(f"\n🎉 Geração de gráficos concluída!")
    print(f"📖 Consulte graphs/README.md para detalhes sobre interpretação")
    
    return 0

if __name__ == '__main__':
    sys.exit(main())
