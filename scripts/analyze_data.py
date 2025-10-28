#!/usr/bin/env python3
"""
Script de análise de dados para o projeto OpenGL Performance Test
Gera gráficos e relatórios baseados nos dados coletados
"""

import pandas as pd
import matplotlib
matplotlib.use('Agg')  # Usar backend sem interface gráfica
import matplotlib.pyplot as plt
import numpy as np
import os
import sys
from datetime import datetime

class PerformanceAnalyzer:
    def __init__(self, csv_file):
        self.csv_file = csv_file
        self.data = None
        self.load_data()
    
    def load_data(self):
        """Carrega dados do arquivo CSV"""
        try:
            self.data = pd.read_csv(self.csv_file)
            print(f"Dados carregados: {len(self.data)} registros")
        except FileNotFoundError:
            print(f"Arquivo não encontrado: {self.csv_file}")
            sys.exit(1)
        except Exception as e:
            print(f"Erro ao carregar dados: {e}")
            sys.exit(1)
    
    def generate_fps_vs_triangles_plot(self):
        """Gera gráfico FPS vs Número de Triângulos"""
        plt.figure(figsize=(12, 8))
        
        # Separar dados por configuração
        basic_data = self.data[self.data['Has_Lighting'] == False]
        lighting_data = self.data[self.data['Has_Lighting'] == True]
        
        if not basic_data.empty:
            plt.plot(basic_data['Triangle_Count'], basic_data['FPS'], 
                    'o-', label='Sem Iluminação', linewidth=2, markersize=6)
        
        if not lighting_data.empty:
            plt.plot(lighting_data['Triangle_Count'], lighting_data['FPS'], 
                    's-', label='Com Iluminação', linewidth=2, markersize=6)
        
        plt.xlabel('Número de Triângulos')
        plt.ylabel('FPS')
        plt.title('Desempenho: FPS vs Número de Triângulos')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        
        # Salvar gráfico
        plt.savefig('reports/fps_vs_triangles.png', dpi=300, bbox_inches='tight')
        plt.close()  # Fechar figura para liberar memória
    
    def generate_cpu_gpu_usage_plot(self):
        """Gera gráfico de uso de CPU e GPU"""
        plt.figure(figsize=(14, 8))
        
        # Subplot 1: CPU Usage
        plt.subplot(2, 1, 1)
        plt.plot(self.data['Triangle_Count'], self.data['CPU_Usage'], 
                'ro-', label='CPU Usage', linewidth=2, markersize=6)
        plt.xlabel('Número de Triângulos')
        plt.ylabel('Uso da CPU (%)')
        plt.title('Uso da CPU vs Número de Triângulos')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        # Subplot 2: GPU Usage
        plt.subplot(2, 1, 2)
        plt.plot(self.data['Triangle_Count'], self.data['GPU_Usage'], 
                'bo-', label='GPU Usage', linewidth=2, markersize=6)
        plt.xlabel('Número de Triângulos')
        plt.ylabel('Uso da GPU (%)')
        plt.title('Uso da GPU vs Número de Triângulos')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('reports/cpu_gpu_usage.png', dpi=300, bbox_inches='tight')
        plt.close()
    
    def generate_lighting_impact_plot(self):
        """Gera gráfico mostrando impacto da iluminação"""
        plt.figure(figsize=(12, 8))
        
        # Separar dados por configuração
        basic_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == False)]
        lighting_data = self.data[(self.data['Has_Lighting'] == True) & (self.data['Has_Textures'] == False)]
        
        if not basic_data.empty:
            plt.plot(basic_data['Triangle_Count'], basic_data['FPS'], 
                    'o-', label='Sem Iluminação', linewidth=2, markersize=6)
        
        if not lighting_data.empty:
            plt.plot(lighting_data['Triangle_Count'], lighting_data['FPS'], 
                    's-', label='Com Iluminação', linewidth=2, markersize=6)
        
        plt.xlabel('Número de Triângulos')
        plt.ylabel('FPS')
        plt.title('Impacto da Iluminação no Desempenho')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('reports/lighting_impact.png', dpi=300, bbox_inches='tight')
        plt.close()
    
    def generate_texture_impact_plot(self):
        """Gera gráfico mostrando impacto das texturas"""
        plt.figure(figsize=(12, 8))
        
        # Separar dados por configuração
        basic_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == False)]
        texture_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == True)]
        
        if not basic_data.empty:
            plt.plot(basic_data['Triangle_Count'], basic_data['FPS'], 
                    'o-', label='Sem Texturas', linewidth=2, markersize=6)
        
        if not texture_data.empty:
            plt.plot(texture_data['Triangle_Count'], texture_data['FPS'], 
                    's-', label='Com Texturas', linewidth=2, markersize=6)
        
        plt.xlabel('Número de Triângulos')
        plt.ylabel('FPS')
        plt.title('Impacto das Texturas no Desempenho')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('reports/texture_impact.png', dpi=300, bbox_inches='tight')
        plt.close()
    
    def generate_comprehensive_comparison_plot(self):
        """Gera gráfico comparativo completo de todos os testes"""
        plt.figure(figsize=(14, 10))
        
        # Separar dados por configuração
        basic_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == False)]
        lighting_data = self.data[(self.data['Has_Lighting'] == True) & (self.data['Has_Textures'] == False)]
        texture_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == True)]
        combined_data = self.data[(self.data['Has_Lighting'] == True) & (self.data['Has_Textures'] == True)]
        
        if not basic_data.empty:
            plt.plot(basic_data['Triangle_Count'], basic_data['FPS'], 
                    'o-', label='Básico (sem efeitos)', linewidth=2, markersize=6, color='blue')
        
        if not lighting_data.empty:
            plt.plot(lighting_data['Triangle_Count'], lighting_data['FPS'], 
                    's-', label='Com Iluminação', linewidth=2, markersize=6, color='orange')
        
        if not texture_data.empty:
            plt.plot(texture_data['Triangle_Count'], texture_data['FPS'], 
                    '^-', label='Com Texturas', linewidth=2, markersize=6, color='green')
        
        if not combined_data.empty:
            plt.plot(combined_data['Triangle_Count'], combined_data['FPS'], 
                    'd-', label='Combinado (Iluminação + Texturas)', linewidth=2, markersize=6, color='red')
        
        plt.xlabel('Número de Triângulos')
        plt.ylabel('FPS')
        plt.title('Comparação Completa: Impacto dos Efeitos Gráficos no Desempenho')
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('reports/comprehensive_comparison.png', dpi=300, bbox_inches='tight')
        plt.close()
    
    def generate_cpu_gpu_impact_plot(self):
        """Gera gráfico mostrando impacto nos recursos de hardware"""
        fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(14, 10))
        
        # Separar dados por configuração
        basic_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == False)]
        lighting_data = self.data[(self.data['Has_Lighting'] == True) & (self.data['Has_Textures'] == False)]
        texture_data = self.data[(self.data['Has_Lighting'] == False) & (self.data['Has_Textures'] == True)]
        combined_data = self.data[(self.data['Has_Lighting'] == True) & (self.data['Has_Textures'] == True)]
        
        # CPU Usage
        if not basic_data.empty:
            ax1.plot(basic_data['Triangle_Count'], basic_data['CPU_Usage'], 
                    'o-', label='Básico', linewidth=2, markersize=4, color='blue')
        if not lighting_data.empty:
            ax1.plot(lighting_data['Triangle_Count'], lighting_data['CPU_Usage'], 
                    's-', label='Com Iluminação', linewidth=2, markersize=4, color='orange')
        if not texture_data.empty:
            ax1.plot(texture_data['Triangle_Count'], texture_data['CPU_Usage'], 
                    '^-', label='Com Texturas', linewidth=2, markersize=4, color='green')
        if not combined_data.empty:
            ax1.plot(combined_data['Triangle_Count'], combined_data['CPU_Usage'], 
                    'd-', label='Combinado', linewidth=2, markersize=4, color='red')
        
        ax1.set_xlabel('Número de Triângulos')
        ax1.set_ylabel('Uso da CPU (%)')
        ax1.set_title('Impacto dos Efeitos Gráficos no Uso da CPU')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # GPU Usage
        if not basic_data.empty:
            ax2.plot(basic_data['Triangle_Count'], basic_data['GPU_Usage'], 
                    'o-', label='Básico', linewidth=2, markersize=4, color='blue')
        if not lighting_data.empty:
            ax2.plot(lighting_data['Triangle_Count'], lighting_data['GPU_Usage'], 
                    's-', label='Com Iluminação', linewidth=2, markersize=4, color='orange')
        if not texture_data.empty:
            ax2.plot(texture_data['Triangle_Count'], texture_data['GPU_Usage'], 
                    '^-', label='Com Texturas', linewidth=2, markersize=4, color='green')
        if not combined_data.empty:
            ax2.plot(combined_data['Triangle_Count'], combined_data['GPU_Usage'], 
                    'd-', label='Combinado', linewidth=2, markersize=4, color='red')
        
        ax2.set_xlabel('Número de Triângulos')
        ax2.set_ylabel('Uso da GPU (%)')
        ax2.set_title('Impacto dos Efeitos Gráficos no Uso da GPU')
        ax2.legend()
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('reports/cpu_gpu_impact.png', dpi=300, bbox_inches='tight')
        plt.close()
    
    def generate_summary_table(self):
        """Gera tabela resumo dos resultados"""
        print("\n=== TABELA RESUMO DOS RESULTADOS ===")
        
        # Estatísticas por número de triângulos
        summary = self.data.groupby('Triangle_Count').agg({
            'FPS': ['mean', 'std', 'min', 'max'],
            'CPU_Usage': ['mean', 'std'],
            'GPU_Usage': ['mean', 'std']
        }).round(2)
        
        print(summary)
        
        # Salvar tabela em arquivo
        with open('reports/summary_table.txt', 'w') as f:
            f.write("=== TABELA RESUMO DOS RESULTADOS ===\n")
            f.write(f"Data: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
            f.write(str(summary))
        
        return summary
    
    def generate_all_plots(self):
        """Gera todos os gráficos"""
        print("Gerando gráficos de análise...")
        
        # Criar diretório de relatórios se não existir
        os.makedirs('reports', exist_ok=True)
        
        # Gerar gráficos
        self.generate_fps_vs_triangles_plot()
        self.generate_cpu_gpu_usage_plot()
        self.generate_lighting_impact_plot()
        self.generate_texture_impact_plot()
        self.generate_comprehensive_comparison_plot()
        self.generate_cpu_gpu_impact_plot()
        
        # Gerar tabela resumo
        self.generate_summary_table()
        
        print("Todos os gráficos foram gerados na pasta 'reports/'")

def main():
    if len(sys.argv) != 2:
        print("Uso: python3 analyze_data.py <arquivo_csv>")
        print("Exemplo: python3 analyze_data.py data/performance_data.csv")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    
    if not os.path.exists(csv_file):
        print(f"Arquivo não encontrado: {csv_file}")
        sys.exit(1)
    
    # Criar analisador e gerar relatórios
    analyzer = PerformanceAnalyzer(csv_file)
    analyzer.generate_all_plots()
    
    print("\nAnálise concluída! Verifique a pasta 'reports/' para os resultados.")

if __name__ == "__main__":
    main()
