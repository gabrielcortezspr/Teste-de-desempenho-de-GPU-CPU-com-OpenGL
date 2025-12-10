#!/bin/bash

# Script de demonstração do projeto OpenGL Performance Test
# Este script facilita a execução do programa interativo

clear

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║              TESTE DE PERFORMANCE GPU/CPU - PDI 3            ║"
echo "║                    Script de Demonstração                     ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

# Verificar se está no diretório correto
if [ ! -f "CMakeLists.txt" ]; then
    echo "❌ ERRO: Execute este script no diretório raiz do projeto!"
    exit 1
fi

# Criar diretório build se não existir
if [ ! -d "build" ]; then
    echo "📁 Criando diretório build..."
    mkdir build
fi

cd build

# Verificar se precisa compilar
if [ ! -f "DemoInterativo" ]; then
    echo "🔨 Compilando projeto pela primeira vez..."
    echo ""
    cmake ..
    make -j$(nproc)
    echo ""
fi

# Verificar se a compilação foi bem-sucedida
if [ ! -f "DemoInterativo" ]; then
    echo "❌ ERRO: Falha na compilação!"
    exit 1
fi

echo "✓ Executável encontrado!"
echo ""
echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║                   INICIANDO DEMO INTERATIVA                   ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
echo "🎮 CONTROLES RÁPIDOS:"
echo "   [1] Modo Básico  [2] Iluminação  [3] Texturas  [4] Combinado"
echo "   [↑/↓] Ajustar triângulos  [R] Reset  [ESC] Sair"
echo ""
echo "Pressione qualquer tecla para iniciar..."
read -n 1 -s

echo ""
echo "🚀 Iniciando aplicação..."
echo ""

# Executar o programa
./DemoInterativo

# Mensagem de encerramento
echo ""
echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║                    DEMONSTRAÇÃO ENCERRADA                     ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
