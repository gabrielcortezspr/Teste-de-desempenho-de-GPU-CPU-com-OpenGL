# Teste Extremo de Performance - README

## Descrição

Este teste extremo coleta dados **reais** de performance do OpenGL variando de **10.000 a 200.000 triângulos** em passos de **10.000**.

## Características do Teste

- **Totalmente Automático**: Basta pressionar ESPAÇO uma vez
- **3 Modos de Renderização**:
  1. **Básico**: Apenas triângulos coloridos (sem efeitos)
  2. **Iluminação**: Triângulos + Phong Lighting (2 luzes)
  3. **Combinado**: Triângulos + Iluminação + Texturas procedurais

- **60 Medições Totais**: 20 pontos por modo (10k, 20k, 30k... até 200k)
- **Dados Coletados**: FPS, uso de CPU, uso de GPU
- **Duração**: ~8-12 minutos

## Como Executar

### Opção 1: Script Automatizado (Recomendado)

```bash
./extreme_test.sh
```

Instruções:
1. Pressione ENTER para iniciar
2. Quando a janela abrir, pressione ESPAÇO
3. **NÃO FECHE A JANELA** - aguarde o teste completar
4. Os gráficos serão gerados automaticamente

### Opção 2: Manual

```bash
cd build
./ExtremeTest
# Pressione ESPAÇO para iniciar
# Aguarde completar (~8-12 minutos)

# Depois gere os gráficos:
cd ..
python3 scripts/analyze_extreme_data.py data/extreme_performance_data.csv
```

## Arquivos Gerados

Após a execução, os seguintes arquivos serão criados:

1. **data/extreme_performance_data.csv**
   - Dados brutos: triângulos, FPS, CPU%, GPU%, modo

2. **reports/extreme_performance_report.txt**
   - Relatório textual com estatísticas

3. **reports/extreme_performance_analysis.png**
   - Gráficos comparativos dos 3 modos

## O Que Observar Durante o Teste

- **Modo 1 (Básico)**: Deve ter FPS mais alto
- **Modo 2 (Iluminação)**: FPS ~13% menor que básico
- **Modo 3 (Combinado)**: FPS ~45% menor que básico (mais pesado)

À medida que os triângulos aumentam:
- FPS diminui progressivamente
- GPU atinge 100% de uso (~30-40k triângulos)
- Sistema fica visivelmente mais lento após 100k triângulos

## Avisos

⚠️ **Este teste é EXTREMO!**
- Pode deixar seu sistema temporariamente lento
- Não execute outros programas pesados simultaneamente
- Certifique-se de salvar trabalhos importantes antes
- GPU integrada pode esquentar (normal)

## Interpretação dos Resultados

### FPS (Frames por Segundo)
- **>30 FPS**: Renderização fluida
- **15-30 FPS**: Perceptivelmente lento
- **<15 FPS**: Muito lento, quase travando

### Uso de GPU
- **<50%**: GPU ociosa, há margem para mais
- **50-90%**: GPU trabalhando adequadamente
- **90-100%**: GPU no limite (gargalo)

### Comparação entre Modos
- **Básico vs Iluminação**: Mostra custo do Phong Lighting
- **Iluminação vs Combinado**: Mostra custo das texturas
- **Básico vs Combinado**: Impacto total de todos os efeitos

## Troubleshooting

**Problema**: Teste não inicia automaticamente
- **Solução**: Pressione ESPAÇO manualmente na janela

**Problema**: GPU não atinge 100%
- **Solução**: Normal em GPUs dedicadas potentes. Em integradas, deveria atingir.

**Problema**: Programa trava em cargas altas
- **Solução**: Normal com >150k triângulos. Aguarde ou pressione ESC para abortar.

**Problema**: Gráficos não são gerados
- **Solução**: Execute manualmente:
  ```bash
  python3 scripts/analyze_extreme_data.py data/extreme_performance_data.csv
  ```

## Próximos Passos

Após coletar os dados:
1. Analise os gráficos em `reports/extreme_performance_analysis.png`
2. Leia o relatório em `reports/extreme_performance_report.txt`
3. Use os dados CSV para análises personalizadas
4. Compare com resultados de outros hardwares

## Atualização do Relatório Final

Para incorporar estes resultados no relatório final:

```bash
# Os dados estão prontos em:
# - data/extreme_performance_data.csv
# - reports/extreme_performance_analysis.png

# Use estes arquivos para atualizar RELATORIO_FINAL_COMPLETO.md
```
