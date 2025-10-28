# INSTRUÇÕES DE ENTREGA - PROJETO OPENGL PERFORMANCE TEST

## ARQUIVOS PARA ENTREGA

### 1. Código-Fonte Completo
- **Pasta:** `src/` - Todo o código C++
- **Pasta:** `scripts/` - Scripts Python de análise
- **Arquivo:** `CMakeLists.txt` - Configuração de build
- **Arquivo:** `README.md` - Documentação completa

### 2. Executáveis Compilados
- **Pasta:** `build/` - Contém os executáveis prontos
- **Arquivos:** `OpenGL_Performance_Test` e `PerformanceTest`

### 3. Dados e Relatórios
- **Pasta:** `data/` - Dados coletados (CSV)
- **Pasta:** `reports/` - Gráficos e relatórios gerados
- **Arquivo:** `RELATORIO_FINAL.md` - Relatório completo

### 4. Scripts de Automação
- **Arquivo:** `setup.sh` - Instalação de dependências
- **Arquivo:** `test.sh` - Teste rápido do sistema

## COMO EXECUTAR O PROJETO

### Opção 1: Usar Executáveis Prontos
```bash
# Teste básico (triângulo único)
cd build
./OpenGL_Performance_Test

# Teste completo (múltiplos triângulos)
./PerformanceTest
```

### Opção 2: Compilar do Zero
```bash
# Instalar dependências
./setup.sh

# Compilar
cd build
cmake ..
make -j$(nproc)

# Executar
./OpenGL_Performance_Test
./PerformanceTest
```

### Opção 3: Teste Automatizado
```bash
# Executa teste completo e gera relatórios
./test.sh
```

## ANÁLISE DE DADOS

Após executar os testes, os dados são salvos em `data/performance_data.csv`.

Para gerar gráficos e análise:
```bash
python3 scripts/analyze_data.py data/performance_data.csv
```

Os gráficos são salvos em `reports/`:
- `fps_vs_triangles.png` - FPS vs Número de Triângulos
- `cpu_gpu_usage.png` - Uso de CPU e GPU
- `lighting_impact.png` - Impacto da Iluminação
- `texture_impact.png` - Impacto das Texturas

## CONTROLES DOS PROGRAMAS

### Programa Básico (`OpenGL_Performance_Test`)
- **ESC:** Sair
- **Observação:** FPS é exibido no console

### Programa de Performance (`PerformanceTest`)
- **ESPAÇO:** Iniciar/Pausar teste
- **R:** Reset do teste
- **ESC:** Sair
- **Observação:** Teste varia de 1 até 1000 triângulos

## REQUISITOS DO SISTEMA

### Mínimos
- Linux (Ubuntu/Debian recomendado)
- OpenGL 3.3+
- 4GB RAM
- Compilador C++ (GCC/Clang)

### Dependências
- GLFW3 (biblioteca de janelas)
- GLEW (carregador OpenGL)
- Python 3 + matplotlib + pandas
- CMake (build system)

## RESULTADOS ESPERADOS

### 1. Triângulo Girando
- Triângulo colorido girando na tela
- FPS estável (~60 FPS em sistema adequado)
- Informações de hardware no console

### 2. Teste de Performance
- Variação automática de triângulos
- Coleta de dados FPS, CPU, GPU
- Geração de arquivo CSV com resultados

### 3. Análise de Dados
- Gráficos mostrando degradação de performance
- Tabelas com estatísticas detalhadas
- Relatórios em texto e imagem

## TROUBLESHOOTING

### Erro de Compilação
```bash
# Verificar dependências
pkg-config --cflags --libs glfw3
pkg-config --cflags --libs glew

# Reinstalar dependências
sudo apt install libglfw3-dev libglew-dev libgl1-mesa-dev
```

### Erro de Execução
```bash
# Verificar OpenGL
glxinfo | grep "OpenGL version"

# Verificar GPU
lspci | grep -i vga
```

### Erro de Análise
```bash
# Instalar Python packages
pip3 install matplotlib pandas numpy
```

## EVIDÊNCIAS DE FUNCIONAMENTO

### 1. Screenshots
- Capturas de tela dos programas executando
- Gráficos gerados pela análise

### 2. Logs de Console
- Saída dos programas mostrando FPS
- Informações de hardware detectadas

### 3. Arquivos de Dados
- `data/performance_data.csv` - Dados brutos
- `reports/*.png` - Gráficos gerados
- `reports/summary_table.txt` - Estatísticas

## VALIDAÇÃO DOS REQUISITOS

### ✅ Requisitos Atendidos
1. **Triângulo colorido girando** - Implementado e funcionando
2. **Cálculo de FPS** - Implementado e exibido no console
3. **Múltiplos triângulos** - Implementado com variação automática
4. **Gráfico FPS vs triângulos** - Gerado automaticamente
5. **Identificação de GPU** - Detecta automaticamente
6. **Monitoramento CPU/GPU** - Implementado e funcional
7. **Geração de relatórios** - Automatizada com gráficos

### 🔄 Requisitos Parciais
1. **Luz omnidirecional** - Estrutura preparada, implementação pendente
2. **Luz spotlight** - Estrutura preparada, implementação pendente
3. **Texturas** - Estrutura preparada, implementação pendente

## NOTA FINAL

O projeto está **FUNCIONAL** e atende aos requisitos principais da disciplina. O sistema:
- ✅ Renderiza triângulos coloridos girando
- ✅ Calcula e exibe FPS
- ✅ Testa performance com múltiplos triângulos
- ✅ Monitora hardware (CPU/GPU)
- ✅ Gera gráficos e relatórios
- ✅ Identifica GPU do sistema
- ✅ Funciona em Linux com OpenGL

**Status:** Pronto para entrega com funcionalidades principais implementadas.
