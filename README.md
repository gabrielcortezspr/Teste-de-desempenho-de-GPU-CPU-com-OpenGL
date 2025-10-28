# OpenGL Performance Test

Este projeto implementa um teste completo de desempenho GPU/CPU usando OpenGL, conforme requisitos da disciplina PDI 3.

## Objetivo

Desenvolver um sistema de teste de desempenho que:
- Renderiza triângulos coloridos girando
- Varia a quantidade de triângulos para testar performance
- Monitora uso de CPU e GPU
- Gera relatórios e gráficos de análise
- Testa impacto de iluminação e texturas

## Estrutura do Projeto

```
PDI 3/
├── src/                          # Código fonte C++
│   ├── main.cpp                  # Programa básico (triângulo único)
│   ├── performance_test.cpp      # Programa de teste de performance
│   ├── Renderer.h/.cpp           # Renderização básica
│   ├── MultiTriangleRenderer.h/.cpp  # Renderização múltiplos triângulos
│   └── PerformanceMonitor.h/.cpp # Monitoramento de hardware
├── scripts/                      # Scripts Python
│   └── analyze_data.py          # Análise de dados e geração de gráficos
├── shaders/                      # Shaders GLSL (futuro)
├── textures/                     # Texturas (futuro)
├── data/                         # Dados coletados (CSV)
├── reports/                      # Relatórios gerados
├── build/                        # Arquivos de compilação
├── CMakeLists.txt               # Configuração CMake
├── setup.sh                     # Script de instalação
└── README.md                    # Este arquivo
```

## Instalação e Compilação

### 1. Instalar Dependências

```bash
# Executar script de instalação
./setup.sh
```

Ou instalar manualmente:
```bash
sudo apt update
sudo apt install -y build-essential cmake libglfw3-dev libglew-dev libgl1-mesa-dev mesa-utils python3 python3-pip python3-matplotlib python3-pandas
pip3 install matplotlib pandas numpy
```

### 2. Compilar Projeto

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Execução

### Teste Básico (Triângulo Único)
```bash
./build/OpenGL_Performance_Test
```

### Teste de Performance (Múltiplos Triângulos)
```bash
./build/PerformanceTest
```

**Controles:**
- `ESPAÇO`: Iniciar/Pausar teste de performance
- `R`: Reset do teste
- `ESC`: Sair

## Análise de Dados

Após executar o teste de performance, os dados são salvos em `data/performance_data.csv`.

Para gerar gráficos e análise:

```bash
python3 scripts/analyze_data.py data/performance_data.csv
```

Os gráficos são salvos na pasta `reports/`:
- `fps_vs_triangles.png`: FPS vs Número de Triângulos
- `cpu_gpu_usage.png`: Uso de CPU e GPU
- `lighting_impact.png`: Impacto da iluminação
- `texture_impact.png`: Impacto das texturas
- `summary_table.txt`: Tabela resumo dos resultados

## Funcionalidades Implementadas

### ✅ Etapa 1: Programa Básico
- [x] Triângulo colorido girando
- [x] Cálculo e exibição de FPS
- [x] Uso de GLFW para janela
- [x] Uso de GLEW para carregar funções OpenGL
- [x] Shaders modernos (OpenGL 3.3+)

### ✅ Etapa 2: Teste de Performance
- [x] Renderização de múltiplos triângulos
- [x] Variação configurável da quantidade
- [x] Registro de FPS médios
- [x] Geração de gráficos FPS × triângulos

### ✅ Etapa 3: Monitoramento de Hardware
- [x] Identificação da GPU
- [x] Verificação de qual GPU está sendo usada
- [x] Monitoramento de % de uso CPU e GPU
- [x] Informações do processador

### 🔄 Etapa 4: Efeitos Gráficos (Em Desenvolvimento)
- [ ] Luz omnidirecional
- [ ] Luz spotlight
- [ ] Texturas nas formas
- [ ] Testes de performance com efeitos

### 🔄 Etapa 5: Relatório Final (Em Desenvolvimento)
- [ ] Relatório completo com metodologia
- [ ] Análise de impacto da iluminação
- [ ] Análise de impacto das texturas
- [ ] Código-fonte como anexo

## Configurações do Teste

No arquivo `src/performance_test.cpp`, você pode ajustar:

```cpp
int maxTriangles = 1000;    // Máximo de triângulos
int triangleStep = 50;      // Incremento entre testes
```

## Requisitos do Sistema

- **Sistema Operacional**: Linux (Ubuntu/Debian recomendado)
- **GPU**: Qualquer GPU com suporte OpenGL 3.3+
- **CPU**: Qualquer processador x86_64
- **RAM**: Mínimo 4GB
- **Dependências**: Ver seção de instalação

## Monitoramento de GPU

O sistema detecta automaticamente:
- **NVIDIA**: Usa `nvidia-smi`
- **AMD**: Usa `radeontop`
- **Intel**: Informações básicas via OpenGL

## Troubleshooting

### Erro de Compilação
```bash
# Verificar dependências
pkg-config --cflags --libs glfw3
pkg-config --cflags --libs glew
```

### Erro de Execução
```bash
# Verificar drivers OpenGL
glxinfo | grep "OpenGL version"
```

### GPU não Detectada
```bash
# Verificar GPU disponível
lspci | grep -i vga
nvidia-smi  # Para NVIDIA
radeontop   # Para AMD
```

## Próximos Passos

1. **Implementar Iluminação**: Adicionar luz omnidirecional e spotlight
2. **Implementar Texturas**: Adicionar texturas aos triângulos
3. **Expandir Testes**: Incluir testes com efeitos gráficos
4. **Melhorar Relatórios**: Adicionar mais análises estatísticas
5. **Interface Gráfica**: Criar interface para configuração dos testes

## Contribuição

Para contribuir com o projeto:
1. Fork o repositório
2. Crie uma branch para sua feature
3. Commit suas mudanças
4. Push para a branch
5. Abra um Pull Request

## Licença

Este projeto é desenvolvido para fins acadêmicos na disciplina PDI 3.

## Contato

Desenvolvido por: [Seu Nome]
Disciplina: PDI 3 - Processamento Digital de Imagens
Instituição: [Nome da Instituição]
