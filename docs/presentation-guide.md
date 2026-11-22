# 🎯 GUIA RÁPIDO DE APRESENTAÇÃO

## ⚡ INICIAR DEMONSTRAÇÃO (MAIS FÁCIL)

### Opção 1: Script Automático
```bash
./scripts/demo.sh
```

### Opção 2: Direto
```bash
cd build
./DemoInterativo
```

---

## 🎮 ROTEIRO DE APRESENTAÇÃO SUGERIDO

### **1. INTRODUÇÃO (30 segundos)**
- Abrir o programa `./DemoInterativo`
- Mostrar a interface inicial com as instruções
- Explicar: "Este programa testa o impacto de efeitos gráficos na performance"

### **2. MODO BÁSICO (1 minuto)**
**Pressione `1`**
- Mostrar triângulos coloridos simples
- Notar FPS alto (~60 FPS)
- Pressionar `↑` várias vezes para aumentar triângulos
- Observar FPS começar a cair
- **Ponto chave**: "Sem efeitos, a GPU processa rapidamente"

### **3. ADICIONAR ILUMINAÇÃO (1 minuto)**
**Pressione `2`**
- Triângulos ganham iluminação realista
- Notar queda no FPS (~10-15% menor)
- Pressionar `↓` para reduzir triângulos
- **Ponto chave**: "Iluminação adiciona cálculos na GPU, reduz FPS"

### **4. ADICIONAR TEXTURAS (1 minuto)**
**Pressione `3`**
- Texturas aparecem sobre os triângulos
- Notar queda maior no FPS (~25% menor)
- **Ponto chave**: "Texturas consomem mais memória e processamento"

### **5. MODO COMBINADO (1 minuto)**
**Pressione `4`**
- Iluminação + Texturas juntas
- Notar a maior queda de FPS (~40-45% menor)
- Testar aumentar/diminuir triângulos
- **Ponto chave**: "Efeitos combinados têm impacto significativo"

### **6. COMPARAÇÃO DIRETA (30 segundos)**
- Ajustar para ~100 triângulos
- Alternar rapidamente entre modos `1`, `2`, `3`, `4`
- Mostrar diferença visual de FPS no console
- **Conclusão**: "Cada efeito tem seu custo de performance"

### **7. TESTE DE LIMITE (30 segundos)**
- Voltar ao modo básico `1`
- Pressionar `↑` até FPS ficar muito baixo
- Mostrar GPU chegando a 100% de uso
- **Ponto final**: "GPU integrada é o gargalo do sistema"

---

## 💬 FRASES PARA USAR NA APRESENTAÇÃO

### Durante Modo Básico:
> "Como podem ver, com triângulos simples conseguimos 60 FPS facilmente. A GPU está processando apenas geometria e cores."

### Ativando Iluminação:
> "Ao adicionar iluminação, note a diferença visual - os triângulos ganham profundidade. Mas observe o FPS cair aproximadamente 13%, pois agora a GPU precisa calcular luz, reflexos e sombras."

### Ativando Texturas:
> "As texturas adicionam detalhes visuais, mas custam mais caro - cerca de 26% de redução no FPS. Isso porque a GPU precisa ler dados da textura e aplicá-los a cada pixel."

### Modo Combinado:
> "No modo combinado, vemos o impacto total: aproximadamente 45% de redução no FPS. Este é o custo real de gráficos realistas modernos."

### Conclusão:
> "Este projeto demonstra claramente como diferentes técnicas gráficas impactam a performance. No desenvolvimento de jogos e aplicações 3D, é essencial balancear qualidade visual com performance."

---

## 📊 DADOS PARA MENCIONAR

### Performance Medida:
- **Modo Básico**: 52 FPS médio
- **Com Iluminação**: 45 FPS (-13%)
- **Com Texturas**: 38 FPS (-26%)
- **Combinado**: 29 FPS (-45%)

### Hardware:
- **CPU**: Intel i7-1355U (12 núcleos)
- **GPU**: Intel Iris Xe Graphics (integrada)
- **Gargalo**: GPU atinge 100% com ~300 triângulos

---

## 🎯 DICAS PARA APRESENTAÇÃO

### ✅ FAZER:
- Alternar entre modos devagar para audiência ver diferença
- Comentar as mudanças de FPS no console
- Mostrar aumento/diminuição de triângulos com setas
- Usar `R` para resetar se perder a conta
- Deixar janela e console visíveis lado a lado

### ❌ EVITAR:
- Não aumentar triângulos rápido demais (pode travar)
- Não deixar muito tempo parado em um modo
- Não esquecer de mencionar os números de FPS
- Não pular o modo básico (é a referência!)

---

## 🚨 SE ALGO DER ERRADO

### Programa Fecha Sozinho:
```bash
cd build
./DemoInterativo
# Verificar mensagens de erro no console
```

### FPS Muito Baixo:
- Pressionar `↓` para reduzir triângulos
- Pressionar `R` para resetar
- Pressionar `1` para voltar ao modo básico

### Travou:
- `ESC` para sair
- Reabrir: `./DemoInterativo`

---

## ⏱️ TIMING TOTAL: ~5-6 minutos

Perfeito para demonstrar todos os conceitos sem cansar a audiência!

**BOA SORTE NA APRESENTAÇÃO! 🚀**
