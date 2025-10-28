#!/bin/bash

# Script de instalação e build do projeto OpenGL Performance Test
# Para Ubuntu/Debian

echo "=== OpenGL Performance Test - Setup ==="

# Verificar se estamos no Ubuntu/Debian
if ! command -v apt &> /dev/null; then
    echo "Este script é otimizado para Ubuntu/Debian. Ajuste conforme sua distribuição."
fi

# Instalar dependências
echo "Instalando dependências..."
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    libglfw3-dev \
    libglew-dev \
    libgl1-mesa-dev \
    mesa-utils \
    python3 \
    python3-pip \
    python3-matplotlib \
    python3-pandas \
    nvidia-utils-470 \
    radeontop

# Instalar dependências Python para análise de dados
echo "Instalando dependências Python..."
pip3 install matplotlib pandas numpy

# Criar diretório de build
echo "Criando diretório de build..."
mkdir -p build
cd build

# Configurar CMake
echo "Configurando projeto com CMake..."
cmake ..

# Compilar
echo "Compilando projeto..."
make -j$(nproc)

echo "=== Setup concluído! ==="
echo "Para executar: ./build/OpenGL_Performance_Test"
echo "Para análise de dados: python3 scripts/analyze_data.py"
