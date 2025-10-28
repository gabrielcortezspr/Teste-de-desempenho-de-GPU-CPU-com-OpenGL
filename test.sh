#!/bin/bash

# Script de teste rápido do projeto OpenGL Performance Test

echo "=== Teste Rápido do Projeto OpenGL Performance Test ==="
echo ""

# Verificar se estamos no diretório correto
if [ ! -f "build/OpenGL_Performance_Test" ]; then
    echo "Erro: Executável não encontrado!"
    echo "Execute 'cd build && make' primeiro."
    exit 1
fi

echo "1. Testando programa básico (triângulo único)..."
echo "   Executando por 5 segundos..."
echo ""

# Executar programa básico por 5 segundos
cd build
timeout 5s ./OpenGL_Performance_Test 2>&1 | head -20

echo ""
echo "2. Verificando arquivos gerados..."
cd ..

if [ -f "data/performance_data.csv" ]; then
    echo "   ✓ Arquivo de dados encontrado"
    echo "   Linhas: $(wc -l < data/performance_data.csv)"
else
    echo "   ✗ Arquivo de dados não encontrado"
fi

if [ -f "reports/fps_vs_triangles.png" ]; then
    echo "   ✓ Gráfico FPS vs Triângulos gerado"
else
    echo "   ✗ Gráfico FPS vs Triângulos não encontrado"
fi

if [ -f "reports/cpu_gpu_usage.png" ]; then
    echo "   ✓ Gráfico CPU/GPU gerado"
else
    echo "   ✗ Gráfico CPU/GPU não encontrado"
fi

echo ""
echo "3. Informações do sistema:"
echo "   GPU detectada: $(lspci | grep -i vga | head -1)"
echo "   OpenGL version: $(glxinfo | grep "OpenGL version" | head -1)"

echo ""
echo "=== TESTE CONCLUÍDO ==="
echo ""
echo "Para executar manualmente:"
echo "  cd build && ./OpenGL_Performance_Test"
echo "  cd build && ./PerformanceTest"
echo ""
echo "Para análise de dados:"
echo "  python3 scripts/analyze_data.py data/performance_data.csv"
