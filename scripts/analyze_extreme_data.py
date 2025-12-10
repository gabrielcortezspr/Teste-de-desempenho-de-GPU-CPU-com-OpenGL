#!/usr/bin/env python3
"""
Script de análise de performance para o teste extremo (10k-200k triângulos)
Gera gráficos detalhados dos 3 modos: Básico, Iluminação, Combinado
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

def analyze_extreme_performance(csv_file):
    """Analisa dados do teste extremo e gera visualizações"""
    
    if not os.path.exists(csv_file):
        print(f"Erro: Arquivo {csv_file} não encontrado!")
        return
    
    # Ler dados
    df = pd.read_csv(csv_file)
    
    # Ajustar nomes das colunas para minúsculas
    df.columns = df.columns.str.lower().str.replace('_', '')
    
    # Renomear colunas para padronizar
    column_mapping = {
        'trianglecount': 'triangles',
        'fps': 'fps',
        'cpuusage': 'cpu_usage',
        'gpuusage': 'gpu_usage',
        'haslighting': 'lighting',
        'hastextures': 'textures'
    }
    df.rename(columns=column_mapping, inplace=True)
    
    print("=" * 70)
    print("ANÁLISE DE PERFORMANCE EXTREMA - PASSOS DE 10.000 TRIÂNGULOS")
    print("=" * 70)
    print(f"\nTotal de medições: {len(df)}")
    print(f"Faixa de triângulos: {df['triangles'].min()} - {df['triangles'].max()}")
    
    # Separar por modo (converter strings booleanas para bool)
    df['lighting'] = df['lighting'].map({'true': True, 'false': False, True: True, False: False})
    df['textures'] = df['textures'].map({'true': True, 'false': False, True: True, False: False})
    
    df_basic = df[(df['lighting'] == False) & (df['textures'] == False)]
    df_lighting = df[(df['lighting'] == True) & (df['textures'] == False)]
    df_combined = df[(df['lighting'] == True) & (df['textures'] == True)]
    
    print(f"\nModo Básico: {len(df_basic)} medições")
    print(f"Modo Iluminação: {len(df_lighting)} medições")
    print(f"Modo Combinado: {len(df_combined)} medições")
    
    # Criar figura com 3 subplots
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle('Teste Extremo de Performance OpenGL\n10.000 - 200.000 Triângulos (Passos de 10k)', 
                 fontsize=16, fontweight='bold')
    
    # Subplot 1: FPS vs Triângulos (todos os modos)
    ax1 = axes[0, 0]
    if len(df_basic) > 0:
        ax1.plot(df_basic['triangles'], df_basic['fps'], 'o-', 
                label='Básico', linewidth=2, markersize=6)
    if len(df_lighting) > 0:
        ax1.plot(df_lighting['triangles'], df_lighting['fps'], 's-', 
                label='Iluminação', linewidth=2, markersize=6)
    if len(df_combined) > 0:
        ax1.plot(df_combined['triangles'], df_combined['fps'], '^-', 
                label='Combinado (Ilum + Text)', linewidth=2, markersize=6)
    
    ax1.set_xlabel('Número de Triângulos', fontsize=12)
    ax1.set_ylabel('FPS (Frames por Segundo)', fontsize=12)
    ax1.set_title('Performance: FPS vs Carga', fontsize=14, fontweight='bold')
    ax1.legend(fontsize=10)
    ax1.grid(True, alpha=0.3)
    ax1.set_xlim(0, df['triangles'].max() * 1.05)
    
    # Subplot 2: Uso de CPU
    ax2 = axes[0, 1]
    if len(df_basic) > 0:
        ax2.plot(df_basic['triangles'], df_basic['cpu_usage'], 'o-', 
                label='Básico', linewidth=2, markersize=6, alpha=0.7)
    if len(df_lighting) > 0:
        ax2.plot(df_lighting['triangles'], df_lighting['cpu_usage'], 's-', 
                label='Iluminação', linewidth=2, markersize=6, alpha=0.7)
    if len(df_combined) > 0:
        ax2.plot(df_combined['triangles'], df_combined['cpu_usage'], '^-', 
                label='Combinado', linewidth=2, markersize=6, alpha=0.7)
    
    ax2.set_xlabel('Número de Triângulos', fontsize=12)
    ax2.set_ylabel('Uso de CPU (%)', fontsize=12)
    ax2.set_title('Utilização de CPU', fontsize=14, fontweight='bold')
    ax2.legend(fontsize=10)
    ax2.grid(True, alpha=0.3)
    ax2.set_ylim(0, 100)
    ax2.axhline(y=100, color='r', linestyle='--', alpha=0.5, label='Máximo')
    
    # Subplot 3: Uso de GPU
    ax3 = axes[1, 0]
    if len(df_basic) > 0:
        ax3.plot(df_basic['triangles'], df_basic['gpu_usage'], 'o-', 
                label='Básico', linewidth=2, markersize=6, alpha=0.7)
    if len(df_lighting) > 0:
        ax3.plot(df_lighting['triangles'], df_lighting['gpu_usage'], 's-', 
                label='Iluminação', linewidth=2, markersize=6, alpha=0.7)
    if len(df_combined) > 0:
        ax3.plot(df_combined['triangles'], df_combined['gpu_usage'], '^-', 
                label='Combinado', linewidth=2, markersize=6, alpha=0.7)
    
    ax3.set_xlabel('Número de Triângulos', fontsize=12)
    ax3.set_ylabel('Uso de GPU (%)', fontsize=12)
    ax3.set_title('Utilização de GPU', fontsize=14, fontweight='bold')
    ax3.legend(fontsize=10)
    ax3.grid(True, alpha=0.3)
    ax3.set_ylim(0, 100)
    ax3.axhline(y=100, color='r', linestyle='--', alpha=0.5, label='Máximo')
    
    # Subplot 4: Estatísticas comparativas
    ax4 = axes[1, 1]
    ax4.axis('off')
    
    stats_text = "ESTATÍSTICAS COMPARATIVAS\n\n"
    
    for mode_name, mode_df in [('BÁSICO', df_basic), 
                                ('ILUMINAÇÃO', df_lighting), 
                                ('COMBINADO', df_combined)]:
        if len(mode_df) > 0:
            stats_text += f"{mode_name}:\n"
            stats_text += f"  FPS Médio: {mode_df['fps'].mean():.2f}\n"
            stats_text += f"  FPS Mínimo: {mode_df['fps'].min():.2f} ({mode_df.loc[mode_df['fps'].idxmin(), 'triangles']:.0f} tri)\n"
            stats_text += f"  FPS Máximo: {mode_df['fps'].max():.2f} ({mode_df.loc[mode_df['fps'].idxmax(), 'triangles']:.0f} tri)\n"
            stats_text += f"  CPU Médio: {mode_df['cpu_usage'].mean():.1f}%\n"
            stats_text += f"  GPU Médio: {mode_df['gpu_usage'].mean():.1f}%\n"
            stats_text += f"  GPU Máximo: {mode_df['gpu_usage'].max():.1f}%\n\n"
    
    # Análise de gargalo
    stats_text += "\nANÁLISE DE GARGALO:\n"
    gpu_bottleneck = df[df['gpu_usage'] > 90]['triangles'].min() if len(df[df['gpu_usage'] > 90]) > 0 else None
    if gpu_bottleneck:
        stats_text += f"  GPU atinge 90%+ em: {gpu_bottleneck:.0f} triângulos\n"
    
    # Impacto dos efeitos
    if len(df_basic) > 0 and len(df_lighting) > 0:
        fps_basic_avg = df_basic['fps'].mean()
        fps_lighting_avg = df_lighting['fps'].mean()
        impact_lighting = ((fps_basic_avg - fps_lighting_avg) / fps_basic_avg) * 100
        stats_text += f"\nIMPACTO DA ILUMINAÇÃO: -{impact_lighting:.1f}%\n"
    
    if len(df_lighting) > 0 and len(df_combined) > 0:
        fps_lighting_avg = df_lighting['fps'].mean()
        fps_combined_avg = df_combined['fps'].mean()
        impact_combined = ((fps_lighting_avg - fps_combined_avg) / fps_lighting_avg) * 100
        stats_text += f"IMPACTO DAS TEXTURAS: -{impact_combined:.1f}%\n"
    
    if len(df_basic) > 0 and len(df_combined) > 0:
        fps_basic_avg = df_basic['fps'].mean()
        fps_combined_avg = df_combined['fps'].mean()
        impact_total = ((fps_basic_avg - fps_combined_avg) / fps_basic_avg) * 100
        stats_text += f"IMPACTO TOTAL (Ilum+Text): -{impact_total:.1f}%\n"
    
    ax4.text(0.1, 0.95, stats_text, transform=ax4.transAxes, 
            fontsize=10, verticalalignment='top', fontfamily='monospace',
            bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
    
    plt.tight_layout()
    
    # Salvar gráfico
    output_file = 'reports/extreme_performance_analysis.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\n✓ Gráficos salvos em: {output_file}")
    
    # Mostrar gráfico
    # plt.show()
    
    # Gerar tabela resumida
    print("\n" + "=" * 70)
    print("TABELA RESUMO")
    print("=" * 70)
    print(f"\n{'Modo':<15} {'Triângulos':<12} {'FPS':<8} {'CPU %':<8} {'GPU %':<8}")
    print("-" * 70)
    
    for mode_name, mode_df in [('Básico', df_basic), 
                                ('Iluminação', df_lighting), 
                                ('Combinado', df_combined)]:
        if len(mode_df) > 0:
            print(f"\n{mode_name}:")
            for _, row in mode_df.iterrows():
                print(f"  {'':<13} {row['triangles']:<12.0f} {row['fps']:<8.2f} "
                      f"{row['cpu_usage']:<8.1f} {row['gpu_usage']:<8.1f}")
    
    print("\n" + "=" * 70)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python3 analyze_extreme_data.py <arquivo.csv>")
        print("Exemplo: python3 analyze_extreme_data.py data/extreme_performance_data.csv")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    analyze_extreme_performance(csv_file)
    print("\n✓ Análise concluída!")
