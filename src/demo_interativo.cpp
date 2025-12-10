#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include <iomanip>
#include "AdvancedRenderer.h"
#include "PerformanceMonitor.h"

// Configurações da janela
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Demo Interativa OpenGL - Teste de Performance";

// Variáveis globais
GLFWwindow* window = nullptr;
AdvancedRenderer* renderer = nullptr;
PerformanceMonitor* perfMonitor = nullptr;

// Controle de FPS
double lastTime = 0.0;
int frameCount = 0;
double fps = 0.0;

// Configurações do demo
int currentTriangleCount = 100;
int minTriangles = 100;
int maxTriangles = 999999; // Sem limite prático
int triangleStep = 100;

// Modos de renderização
enum RenderMode {
    MODE_BASICO = 1,
    MODE_ILUMINACAO = 2,
    MODE_TEXTURAS = 3,
    MODE_COMBINADO = 4
};

RenderMode currentMode = MODE_BASICO;
std::string modeNames[] = {
    "",
    "BÁSICO (Sem Efeitos)",
    "ILUMINAÇÃO (Luz Omnidirecional + Spotlight)",
    "TEXTURAS (Texturas Procedurais)",
    "COMBINADO (Iluminação + Texturas)"
};

bool showHelp = true;
double lastModeChangeTime = 0.0;

// Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback para teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        double currentTime = glfwGetTime();
        
        switch(key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
                
            case GLFW_KEY_1:
                currentMode = MODE_BASICO;
                renderer->setLightingEnabled(false);
                renderer->setTexturesEnabled(false);
                lastModeChangeTime = currentTime;
                std::cout << "\n>>> MODO: " << modeNames[currentMode] << std::endl;
                break;
                
            case GLFW_KEY_2:
                currentMode = MODE_ILUMINACAO;
                renderer->setLightingEnabled(true);
                renderer->setTexturesEnabled(false);
                lastModeChangeTime = currentTime;
                std::cout << "\n>>> MODO: " << modeNames[currentMode] << std::endl;
                break;
                
            case GLFW_KEY_3:
                currentMode = MODE_TEXTURAS;
                renderer->setLightingEnabled(false);
                renderer->setTexturesEnabled(true);
                lastModeChangeTime = currentTime;
                std::cout << "\n>>> MODO: " << modeNames[currentMode] << std::endl;
                break;
                
            case GLFW_KEY_4:
                currentMode = MODE_COMBINADO;
                renderer->setLightingEnabled(true);
                renderer->setTexturesEnabled(true);
                lastModeChangeTime = currentTime;
                std::cout << "\n>>> MODO: " << modeNames[currentMode] << std::endl;
                break;
                
            case GLFW_KEY_UP:
            case GLFW_KEY_KP_ADD:
            case GLFW_KEY_EQUAL:
                currentTriangleCount = std::min(currentTriangleCount + triangleStep, maxTriangles);
                renderer->setTriangleCount(currentTriangleCount);
                std::cout << "Triângulos: " << currentTriangleCount << std::endl;
                break;
                
            case GLFW_KEY_DOWN:
            case GLFW_KEY_KP_SUBTRACT:
            case GLFW_KEY_MINUS:
                currentTriangleCount = std::max(currentTriangleCount - triangleStep, minTriangles);
                renderer->setTriangleCount(currentTriangleCount);
                std::cout << "Triângulos: " << currentTriangleCount << std::endl;
                break;
                
            case GLFW_KEY_H:
                showHelp = !showHelp;
                break;
                
            case GLFW_KEY_R:
                currentTriangleCount = 100;
                currentMode = MODE_BASICO;
                renderer->setTriangleCount(currentTriangleCount);
                renderer->setLightingEnabled(false);
                renderer->setTexturesEnabled(false);
                std::cout << "\n>>> RESETADO para modo básico com 100 triângulos" << std::endl;
                break;
        }
    }
}

// Inicializar GLFW e criar janela
bool initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    return true;
}

// Inicializar GLEW
bool initializeGLEW() {
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao inicializar GLEW" << std::endl;
        return false;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

    return true;
}

// Calcular e exibir FPS
void updateFPS() {
    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 1.0) {
        fps = frameCount / (currentTime - lastTime);
        
        // Exibir informações no console
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "FPS: " << fps 
                  << " | Triângulos: " << currentTriangleCount
                  << " | Modo: " << modeNames[currentMode]
                  << std::endl;
        
        frameCount = 0;
        lastTime = currentTime;
    }
}

// Renderizar interface de ajuda na tela (usando console)
void displayHelp() {
    if (!showHelp) return;
    
    double currentTime = glfwGetTime();
    
    // Mostrar ajuda apenas nos primeiros 5 segundos ou quando H for pressionado
    static bool helpShownOnce = false;
    if (!helpShownOnce || (currentTime - lastModeChangeTime < 3.0)) {
        if (!helpShownOnce) {
            std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
            std::cout << "║          DEMO INTERATIVA - TESTE DE PERFORMANCE GPU/CPU       ║" << std::endl;
            std::cout << "╠═══════════════════════════════════════════════════════════════╣" << std::endl;
            std::cout << "║  CONTROLES:                                                   ║" << std::endl;
            std::cout << "║  ───────────                                                  ║" << std::endl;
            std::cout << "║  [1] → Modo BÁSICO (sem efeitos)                             ║" << std::endl;
            std::cout << "║  [2] → Modo ILUMINAÇÃO (luz omnidirecional + spotlight)      ║" << std::endl;
            std::cout << "║  [3] → Modo TEXTURAS (texturas procedurais)                  ║" << std::endl;
            std::cout << "║  [4] → Modo COMBINADO (iluminação + texturas)                ║" << std::endl;
            std::cout << "║                                                               ║" << std::endl;
            std::cout << "║  [↑] ou [+] → Aumentar triângulos (+100)                     ║" << std::endl;
            std::cout << "║  [↓] ou [-] → Diminuir triângulos (-100)                     ║" << std::endl;
            std::cout << "║                                                               ║" << std::endl;
            std::cout << "║  [R] → Resetar (volta ao modo básico, 50 triângulos)         ║" << std::endl;
            std::cout << "║  [H] → Mostrar/Ocultar esta ajuda                            ║" << std::endl;
            std::cout << "║  [ESC] → Sair                                                 ║" << std::endl;
            std::cout << "╠═══════════════════════════════════════════════════════════════╣" << std::endl;
            std::cout << "║  OBJETIVO:                                                    ║" << std::endl;
            std::cout << "║  Este programa demonstra o impacto de diferentes efeitos      ║" << std::endl;
            std::cout << "║  gráficos na performance da GPU/CPU.                          ║" << std::endl;
            std::cout << "║                                                               ║" << std::endl;
            std::cout << "║  Experimente:                                                 ║" << std::endl;
            std::cout << "║  • Alterne entre os modos e observe a mudança no FPS         ║" << std::endl;
            std::cout << "║  • Aumente os triângulos e veja como afeta a performance     ║" << std::endl;
            std::cout << "║  • Compare modo básico vs combinado com muitos triângulos    ║" << std::endl;
            std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;
            std::cout << "\n>>> Iniciando em MODO BÁSICO com " << currentTriangleCount << " triângulos..." << std::endl;
            std::cout << ">>> Pressione as teclas de 1 a 4 para alternar entre os modos!" << std::endl;
            std::cout << ">>> AVISO: Sem limite de triângulos - cuidado com valores muito altos!\n" << std::endl;
            helpShownOnce = true;
        }
    }
}

// Loop principal de renderização
void renderLoop() {
    double lastFrameTime = glfwGetTime();
    
    displayHelp();
    
    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        // Limpar buffer
        glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar triângulos
        if (renderer) {
            renderer->render(deltaTime);
        }

        // Atualizar monitoramento de performance
        if (perfMonitor) {
            perfMonitor->update();
        }

        // Calcular e exibir FPS
        updateFPS();

        // Trocar buffers e processar eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main() {
    std::cout << "\n";
    std::cout << "╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                   DEMO INTERATIVA OPENGL                      ║" << std::endl;
    std::cout << "║           Teste de Performance GPU/CPU - PDI 3               ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    // Inicializar GLFW
    if (!initializeGLFW()) {
        return -1;
    }

    // Inicializar GLEW
    if (!initializeGLEW()) {
        glfwTerminate();
        return -1;
    }

    // Habilitar teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // Criar renderer avançado
    renderer = new AdvancedRenderer();
    if (!renderer->initialize()) {
        std::cerr << "Erro ao inicializar renderer" << std::endl;
        delete renderer;
        glfwTerminate();
        return -1;
    }

    // Configurar número inicial de triângulos
    renderer->setTriangleCount(currentTriangleCount);

    // Criar monitor de performance
    perfMonitor = new PerformanceMonitor();
    perfMonitor->initialize();

    std::cout << "\n✓ Inicialização concluída com sucesso!" << std::endl;
    std::cout << "✓ Aguarde carregamento da interface...\n" << std::endl;

    // Loop principal
    renderLoop();

    // Limpeza
    delete renderer;
    delete perfMonitor;
    glfwTerminate();

    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    PROGRAMA FINALIZADO                        ║" << std::endl;
    std::cout << "║                  Obrigado por testar!                         ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    return 0;
}
