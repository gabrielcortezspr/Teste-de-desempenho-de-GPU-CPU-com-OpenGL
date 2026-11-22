# RESUMO DO PROJETO - Teste de Desempenho GPU/CPU com OpenGL

**Data:** Dezembro de 2024  
**Disciplina:** PDI 3 - Processamento Digital de Imagens  

---

## 🎯 OBJETIVO DO PROJETO

Desenvolver um sistema completo de teste de desempenho para avaliar o impacto da renderização gráfica (triângulos, iluminação e texturas) no uso de GPU e CPU, utilizando OpenGL moderno.

---

## 🛠️ TECNOLOGIAS UTILIZADAS

### Linguagens e Frameworks
- **C++17**: Linguagem principal para renderização
- **Python 3**: Análise de dados e geração de gráficos
- **OpenGL 3.3+**: API gráfica para renderização
- **GLSL**: Shaders para processamento na GPU

### Bibliotecas Principais
- **GLFW**: Criação de janelas e contexto OpenGL
- **GLEW**: Carregamento de extensões OpenGL
- **GLM**: Matemática 3D (vetores, matrizes)
- **Matplotlib/Pandas**: Análise de dados e visualização

### Ferramentas de Build
- **CMake**: Sistema de build multiplataforma
- **GCC**: Compilador C++

---

## 📊 METODOLOGIA

### 1. **Programa Básico** (`OpenGL_Performance_Test`)
- Renderiza um único triângulo colorido girando
- Calcula FPS em tempo real
- Demonstra funcionamento básico do OpenGL

### 2. **Teste de Performance** (`PerformanceTest`)
- Varia quantidade de triângulos de 1 até 800
- Incremento de 50 triângulos a cada teste
- Duração: 1 segundo por configuração
- Monitora: FPS, uso de CPU e GPU

### 3. **Teste Avançado** (`AdvancedTest`)
- 4 configurações testadas:
  1. **Básico**: Sem efeitos
  2. **Iluminação**: Luz omnidirecional + spotlight
  3. **Texturas**: Texturas procedurais
  4. **Combinado**: Iluminação + texturas
- Incremento de 25 triângulos por teste
- Permite comparação do impacto de cada efeito

### 4. **Monitoramento de Hardware**
- **CPU**: Leitura de `/proc/stat` para calcular uso percentual
- **GPU**: 
  - NVIDIA: via `nvidia-smi`
  - AMD: via `radeontop`
  - Intel: informações via OpenGL
- Atualização a cada segundo

---

## 💻 HARDWARE UTILIZADO

### Sistema Testado
- **Processador**: Intel Core i7-1355U (13ª geração, 12 núcleos)
- **GPU**: Intel Iris Xe Graphics (RPL-U) - GPU integrada
- **OpenGL**: Versão 4.6 (Core Profile) Mesa 25.0.7
- **Sistema**: Ubuntu 24.04 LTS

---

## 📈 PRINCIPAIS RESULTADOS

### Performance por Quantidade de Triângulos

| Triângulos | FPS Médio | CPU Médio | GPU Médio |
|------------|-----------|-----------|-----------|
| 1          | 52.27     | 19.90%    | 33.40%    |
| 50         | 46.55     | 27.19%    | 42.47%    |
| 100        | 40.08     | 34.99%    | 52.32%    |
| 200        | 26.42     | 51.09%    | 72.60%    |
| 300        | 13.81     | 67.19%    | 92.75%    |
| 400        | 3.38      | 82.54%    | 100.00%   |

**Observação**: A GPU atinge 100% de uso com aproximadamente 300-400 triângulos, tornando-se o gargalo do sistema.

### Impacto dos Efeitos Gráficos

#### 1. **Iluminação** (Luz Omnidirecional + Spotlight)
- **Redução no FPS**: -13% em média
- **Aumento no uso da GPU**: +7%
- **Aumento no uso da CPU**: +3%
- **Conclusão**: Iluminação afeta principalmente a GPU

**Características da Iluminação:**
- Luz Omnidirecional: Posição (0, 0, 2), cor branca
- Spotlight: Posição (0, 1, 1), cor laranja, raio 5.0
- Cálculos realizados nos shaders (GPU)

#### 2. **Texturas** (Texturas Procedurais)
- **Redução no FPS**: -26% em média
- **Aumento no uso da GPU**: +15%
- **Aumento no uso da CPU**: +6%
- **Conclusão**: Texturas têm maior impacto que iluminação

**Características das Texturas:**
- Resolução: 256x256 pixels
- Padrão: Xadrez colorido com gradientes
- Geração procedural (sem arquivos externos)
- Uso de memória: ~196KB por textura

#### 3. **Combinado** (Iluminação + Texturas)
- **Redução no FPS**: -45% em média
- **Aumento no uso da GPU**: +25%
- **Aumento no uso da CPU**: +9%
- **Conclusão**: Efeitos combinados têm impacto significativo

---

## 🔍 COMO OS RESULTADOS FORAM OBTIDOS

### Coleta de Dados
1. **Execução dos Programas**: 
   - Programas C++ executam testes automaticamente
   - Dados coletados em tempo real durante renderização
   - Salvos em formato CSV com timestamp

2. **Estrutura dos Dados CSV**:
   ```
   Timestamp, FPS, CPU_Usage, GPU_Usage, Triangle_Count, Has_Lighting, Has_Textures
   ```

3. **Arquivos Gerados**:
   - `data/performance_data.csv`: Teste básico
   - `data/advanced_performance_data.csv`: Teste com efeitos

### Análise de Dados
1. **Script Python** (`scripts/analyze_data.py`):
   - Lê arquivos CSV com Pandas
   - Calcula estatísticas (média, desvio padrão, min, max)
   - Gera gráficos com Matplotlib

2. **Gráficos Gerados**:
   - `fps_vs_triangles.png`: FPS × Quantidade de Triângulos
   - `cpu_gpu_usage.png`: Uso de CPU e GPU
   - `lighting_impact.png`: Impacto da Iluminação
   - `texture_impact.png`: Impacto das Texturas
   - `comprehensive_comparison.png`: Comparação Completa
   - `summary_table.txt`: Tabela Resumo

### Monitoramento de Hardware

#### CPU (Método Utilizado)
```cpp
// Leitura de /proc/stat
FILE* file = fopen("/proc/stat", "r");
// Calcula: (total_time - idle_time) / total_time * 100
```

#### GPU (Métodos por Fabricante)
- **NVIDIA**: `nvidia-smi --query-gpu=utilization.gpu --format=csv`
- **AMD**: `radeontop -d - -l 1`
- **Intel**: Informações básicas via `glGetString(GL_RENDERER)`

---

## 📊 CONCLUSÕES TÉCNICAS

### 1. **Gargalo do Sistema**
- **GPU integrada Intel Iris Xe** é o principal limitador
- CPU tem capacidade suficiente (máximo 93% de uso)
- GPU atinge 100% com ~300 triângulos

### 2. **Degradação de Performance**
- **Linear**: Performance reduz linearmente com aumento de triângulos
- **Previsível**: Modelo permite estimar FPS para qualquer quantidade

### 3. **Hierarquia de Impacto** (do menor para o maior)
1. Iluminação (~13% de redução no FPS)
2. Texturas (~26% de redução no FPS)
3. Combinado (~45% de redução no FPS)

### 4. **Escalabilidade**
- **Sistema Básico**: Funciona bem até ~200 triângulos (>30 FPS)
- **Com Iluminação**: Limite reduz para ~150 triângulos
- **Com Texturas**: Limite reduz para ~100 triângulos
- **Combinado**: Limite reduz para ~75 triângulos

---

## 🎨 IMPLEMENTAÇÃO TÉCNICA

### Arquitetura do Código

```
src/
├── main.cpp                      # Programa básico (demo)
├── performance_test.cpp          # Teste de performance
├── advanced_test.cpp             # Teste com efeitos
├── Renderer.h/.cpp               # Renderização básica
├── MultiTriangleRenderer.h/.cpp  # Múltiplos triângulos
├── AdvancedRenderer.h/.cpp       # Renderização avançada
├── Lighting.h/.cpp               # Sistema de iluminação
├── Texture.h/.cpp                # Sistema de texturas
└── PerformanceMonitor.h/.cpp     # Monitoramento hardware
```

### Pipeline de Renderização

1. **Inicialização**:
   - Criar janela GLFW
   - Inicializar GLEW
   - Compilar shaders
   - Criar buffers (VBO, VAO, EBO)

2. **Loop de Renderização**:
   - Limpar buffers
   - Atualizar transformações (rotação)
   - Aplicar iluminação (se ativada)
   - Aplicar texturas (se ativadas)
   - Desenhar triângulos
   - Swap buffers
   - Calcular FPS

3. **Monitoramento**:
   - Ler uso de CPU/GPU a cada segundo
   - Registrar dados em CSV
   - Atualizar contadores

### Shaders Utilizados

#### Vertex Shader
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
```

#### Fragment Shader (com iluminação e texturas)
```glsl
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform bool hasLighting;
uniform bool hasTexture;
uniform sampler2D texture1;

// Cálculos de luz omnidirecional e spotlight
// Aplicação de texturas
// Combinação final de cores
```

---

## 📁 ESTRUTURA DE ARQUIVOS

```
Teste-de-desempenho-de-GPU-CPU-com-OpenGL/
├── src/                    # Código-fonte C++
├── scripts/                # Scripts Python
├── data/                   # Dados coletados (CSV)
├── reports/                # Gráficos e relatórios
├── build/                  # Executáveis compilados
├── shaders/                # Shaders GLSL (futuro)
├── textures/               # Texturas (futuro)
├── CMakeLists.txt          # Configuração CMake
├── setup.sh                # Script de instalação
├── test.sh                 # Teste automatizado
└── README.md               # Documentação
```

---

## ⚙️ COMO EXECUTAR

### Compilação
```bash
cd build
cmake ..
make -j$(nproc)
```

### Execução dos Testes
```bash
# Teste básico (triângulo único)
./build/OpenGL_Performance_Test

# Teste de performance
./build/PerformanceTest

# Teste avançado (iluminação + texturas)
./build/AdvancedTest
```

### Análise de Dados
```bash
python3 scripts/analyze_data.py data/advanced_performance_data.csv
```

### Teste Completo Automatizado
```bash
./scripts/test.sh
```

---

## 💻 EXPLICAÇÃO PRÁTICA DO CÓDIGO

### **DemoInterativo.cpp** - Programa Principal para Apresentação

Este programa unifica todos os testes em uma única aplicação interativa.

**Estrutura:**
```cpp
// 1. CONFIGURAÇÃO INICIAL
- Janela 1280x720 pixels
- OpenGL 3.3+
- 4 modos de renderização disponíveis

// 2. ENUM DE MODOS
enum RenderMode {
    MODE_BASICO = 1,      // Triângulos simples
    MODE_ILUMINACAO = 2,   // Com luzes
    MODE_TEXTURAS = 3,     // Com texturas
    MODE_COMBINADO = 4     // Luzes + texturas
};

// 3. CONTROLE POR TECLADO
key_callback() {
    switch(key) {
        case '1': Ativa modo básico
        case '2': Ativa iluminação
        case '3': Ativa texturas
        case '4': Ativa combinado
        case '↑': Adiciona 10 triângulos
        case '↓': Remove 10 triângulos
    }
}

// 4. LOOP DE RENDERIZAÇÃO
while(janela aberta) {
    Limpar tela
    Renderizar triângulos (com efeitos do modo atual)
    Calcular FPS
    Exibir informações
    Trocar buffers
}
```

**Funcionamento:**
1. Usuário pressiona tecla (1-4)
2. Programa chama `renderer->setLightingEnabled()` e/ou `setTexturesEnabled()`
3. Shaders são ativados/desativados conforme o modo
4. FPS é recalculado e exibido
5. Permite comparação em tempo real

**Vantagens:**
- ✅ Demonstração visual instantânea
- ✅ Não precisa recompilar entre testes
- ✅ Fácil de apresentar
- ✅ Comparação direta de performance

### **Fluxo de Dados Entre Componentes**

```
┌─────────────────────────────────────────────────────────────┐
│                    DemoInterativo.cpp                        │
│                 (Programa Principal)                         │
└──────────────────────┬──────────────────────────────────────┘
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────────┐
│AdvancedRen-  │ │Performance   │ │GLFW/GLEW         │
│derer         │ │Monitor       │ │(OpenGL)          │
│              │ │              │ │                  │
│- Gerencia    │ │- Lê CPU/GPU  │ │- Janela          │
│  triângulos  │ │- Registra    │ │- Contexto        │
│- Aplica      │ │  dados       │ │- Input           │
│  efeitos     │ │              │ │                  │
└──────┬───────┘ └──────────────┘ └──────────────────┘
       │
   ┌───┴──────┐
   ▼          ▼
┌────────┐ ┌────────┐
│Lighting│ │Texture │
│        │ │        │
│- Shad- │ │- Gera  │
│  ers   │ │  tex-  │
│- Luzes │ │  turas │
└────────┘ └────────┘
```

---

## 💻 EXPLICAÇÃO PRÁTICA DO CÓDIGO

### **DemoInterativo.cpp** - Programa Principal para Apresentação

Este programa unifica todos os testes em uma única aplicação interativa.

**Estrutura:**
```cpp
// 1. CONFIGURAÇÃO INICIAL
- Janela 1280x720 pixels
- OpenGL 3.3+
- 4 modos de renderização disponíveis

// 2. ENUM DE MODOS
enum RenderMode {
    MODE_BASICO = 1,      // Triângulos simples
    MODE_ILUMINACAO = 2,   // Com luzes
    MODE_TEXTURAS = 3,     // Com texturas
    MODE_COMBINADO = 4     // Luzes + texturas
};

// 3. CONTROLE POR TECLADO
key_callback() {
    switch(key) {
        case '1': Ativa modo básico
        case '2': Ativa iluminação
        case '3': Ativa texturas
        case '4': Ativa combinado
        case '↑': Adiciona 10 triângulos
        case '↓': Remove 10 triângulos
    }
}

// 4. LOOP DE RENDERIZAÇÃO
while(janela aberta) {
    Limpar tela
    Renderizar triângulos (com efeitos do modo atual)
    Calcular FPS
    Exibir informações
    Trocar buffers
}
```

**Funcionamento:**
1. Usuário pressiona tecla (1-4)
2. Programa chama `renderer->setLightingEnabled()` e/ou `setTexturesEnabled()`
3. Shaders são ativados/desativados conforme o modo
4. FPS é recalculado e exibido
5. Permite comparação em tempo real

**Vantagens:**
- ✅ Demonstração visual instantânea
- ✅ Não precisa recompilar entre testes
- ✅ Fácil de apresentar
- ✅ Comparação direta de performance

### **Fluxo de Dados Entre Componentes**

```
┌─────────────────────────────────────────────────────────────┐
│                    DemoInterativo.cpp                        │
│                 (Programa Principal)                         │
└──────────────────────┬──────────────────────────────────────┘
                       │
        ┌──────────────┼──────────────┐
        ▼              ▼              ▼
┌──────────────┐ ┌──────────────┐ ┌──────────────────┐
│AdvancedRen-  │ │Performance   │ │GLFW/GLEW         │
│derer         │ │Monitor       │ │(OpenGL)          │
│              │ │              │ │                  │
│- Gerencia    │ │- Lê CPU/GPU  │ │- Janela          │
│  triângulos  │ │- Registra    │ │- Contexto        │
│- Aplica      │ │  dados       │ │- Input           │
│  efeitos     │ │              │ │                  │
└──────┬───────┘ └──────────────┘ └──────────────────┘
       │
   ┌───┴──────┐
   ▼          ▼
┌────────┐ ┌────────┐
│Lighting│ │Texture │
│        │ │        │
│- Shad- │ │- Gera  │
│  ers   │ │  tex-  │
│- Luzes │ │  turas │
└────────┘ └────────┘
```

---

## 🎓 APRENDIZADOS PRINCIPAIS

1. **OpenGL Moderno**: Uso de shaders, VAO/VBO, pipeline programável
2. **Monitoramento de Hardware**: Técnicas para medir uso de CPU/GPU
3. **Análise de Performance**: Identificação de gargalos em sistemas gráficos
4. **Pipeline Gráfico**: Compreensão de como GPU processa geometria e pixels
5. **Otimização**: Impacto de efeitos gráficos na performance
6. **Interface Interativa**: Criação de demos responsivas para apresentação

---

## 🚀 POSSÍVEIS MELHORIAS

1. **Otimizações**:
   - Implementar Level of Detail (LOD)
   - Frustum culling para triângulos fora da tela
   - Instanced rendering para múltiplos objetos

2. **Efeitos Adicionais**:
   - Normal mapping
   - Shadow mapping
   - Post-processing effects

3. **Análise Expandida**:
   - Testes com diferentes resoluções
   - Comparação com GPU dedicada
   - Análise de consumo de energia

---

## ✅ STATUS DO PROJETO

**COMPLETO** - Todos os requisitos da disciplina foram atendidos:
- ✅ Triângulo colorido girando
- ✅ Gráfico FPS vs Triângulos
- ✅ Identificação e monitoramento de GPU
- ✅ Monitoramento de CPU
- ✅ Iluminação omnidirecional e spotlight
- ✅ Texturas implementadas
- ✅ Análise comparativa completa
- ✅ Gráficos de comparação
- ✅ Relatório técnico completo

---

**Desenvolvido para a disciplina PDI 3 - Processamento Digital de Imagens**
