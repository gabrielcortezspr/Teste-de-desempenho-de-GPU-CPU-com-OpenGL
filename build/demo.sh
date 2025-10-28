#!/bin/bash

# Script de demonstração do projeto OpenGL Performance Test
# Executa os testes de forma controlada e gera relatórios

echo "=== OpenGL Performance Test - Demonstração ==="
echo ""

# Verificar se os executáveis existem
if [ ! -f "./OpenGL_Performance_Test" ]; then
    echo "Erro: OpenGL_Performance_Test não encontrado!"
    echo "Execute 'make' primeiro para compilar o projeto."
    exit 1
fi

if [ ! -f "./PerformanceTest" ]; then
    echo "Erro: PerformanceTest não encontrado!"
    echo "Execute 'make' primeiro para compilar o projeto."
    exit 1
fi

# Criar diretórios necessários
mkdir -p ../data ../reports

echo "1. Testando programa básico (triângulo único)..."
echo "   Pressione ESC para sair após alguns segundos"
echo ""

# Executar programa básico por 10 segundos
timeout 10s ./OpenGL_Performance_Test || true

echo ""
echo "2. Executando teste de performance completo..."
echo "   Este teste irá variar de 1 até 1000 triângulos"
echo "   Os dados serão salvos em data/performance_data.csv"
echo ""

# Executar teste de performance
./PerformanceTest

echo ""
echo "3. Gerando análise de dados..."

# Verificar se o arquivo de dados foi criado
if [ -f "../data/performance_data.csv" ]; then
    echo "   Dados encontrados! Gerando gráficos..."
    python3 ../scripts/analyze_data.py ../data/performance_data.csv
    
    echo ""
    echo "=== RELATÓRIOS GERADOS ==="
    echo "Dados brutos: data/performance_data.csv"
    echo "Gráficos: reports/"
    echo "Relatório: reports/performance_report.txt"
    echo ""
    
    # Mostrar resumo dos arquivos gerados
    echo "Arquivos gerados:"
    ls -la ../reports/
    
else
    echo "   Erro: Arquivo de dados não foi criado!"
    echo "   Execute o PerformanceTest manualmente para coletar dados."
fi

echo ""
echo "=== DEMONSTRAÇÃO CONCLUÍDA ==="
echo ""
echo "Para executar manualmente:"
echo "  ./OpenGL_Performance_Test    # Triângulo único"
echo "  ./PerformanceTest            # Teste completo"
echo ""
echo "Para análise de dados:"
echo "  python3 ../scripts/analyze_data.py ../data/performance_data.csv"
