#!/bin/bash

echo "=================================================="
echo "  TESTE EXTREMO DE PERFORMANCE - PASSOS DE 10K  "
echo "=================================================="
echo ""
echo "Este script executará o teste extremo automático"
echo "que varia de 10.000 a 200.000 triângulos"
echo ""
echo "⚠️  ATENÇÃO: Este teste é MUITO PESADO!"
echo "   - Pode deixar seu sistema lento temporariamente"
echo "   - Levará aproximadamente 8-12 minutos"
echo "   - Testará 3 modos:"
echo "     1. Básico (sem efeitos)"
echo "     2. Iluminação"
echo "     3. Combinado (Iluminação + Texturas)"
echo ""
echo "   - Cada modo testa de 10k a 200k triângulos (20 medições)"
echo "   - Total: 60 medições de performance"
echo ""
echo "INSTRUÇÕES:"
echo "  1. A janela do teste abrirá"
echo "  2. Pressione ESPAÇO para iniciar"
echo "  3. Aguarde o teste completar automaticamente (NÃO FECHE A JANELA)"
echo "  4. Os resultados serão salvos em:"
echo "     - data/extreme_performance_data.csv"
echo "     - reports/extreme_performance_report.txt"
echo "     - reports/extreme_performance_analysis.png (gráficos)"
echo ""
read -p "Pressione ENTER para continuar ou Ctrl+C para cancelar..."

cd "$(dirname "$0")/build"

if [ ! -f "./ExtremeTest" ]; then
    echo "Erro: ExtremeTest não encontrado. Compilando..."
    cd ..
    mkdir -p build
    cd build
    cmake .. && make -j$(nproc)
    if [ $? -ne 0 ]; then
        echo "Erro na compilação!"
        exit 1
    fi
fi

echo ""
echo "Iniciando teste extremo..."
echo ""

./ExtremeTest

if [ -f "../data/extreme_performance_data.csv" ]; then
    echo ""
    echo "✓ Dados coletados com sucesso!"
    echo ""
    echo "Gerando gráficos com Python..."
    cd ..
    python3 scripts/analyze_extreme_data.py data/extreme_performance_data.csv
    echo ""
    echo "✓ Teste completo!"
    echo ""
    echo "Arquivos gerados:"
    echo "  - data/extreme_performance_data.csv"
    echo "  - reports/extreme_performance_report.txt"
    echo "  - reports/extreme_performance_analysis.png"
    echo ""
    echo "Para visualizar os gráficos:"
    echo "  xdg-open reports/extreme_performance_analysis.png"
    echo ""
else
    echo ""
    echo "⚠️  Teste não completou ou foi cancelado"
    echo ""
fi
