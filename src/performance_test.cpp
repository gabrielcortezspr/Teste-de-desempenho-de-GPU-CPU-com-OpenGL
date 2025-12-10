#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include "MultiTriangleRenderer.h"
#include "PerformanceMonitor.h"

// Configurações da janela
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "OpenGL Performance Test - Múltiplos Triângulos";

// Variáveis globais
GLFWwindow* window = nullptr;
MultiTriangleRenderer* renderer = nullptr;
PerformanceMonitor* perfMonitor = nullptr;

// Controle de FPS
double lastTime = 0.0;
int frameCount = 0;
double fps = 0.0;

// Configurações do teste
int currentTriangleCount = 1;
int maxTriangles = 1000;
int triangleStep = 50;
bool testRunning = false;
bool testComplete = false;

// Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback para teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        testRunning = !testRunning;
        if (testRunning) {
            std::cout << "\n=== INICIANDO TESTE DE PERFORMANCE ===" << std::endl;
            currentTriangleCount = 1;
            testComplete = false;
        } else {
            std::cout << "\nTeste pausado. Pressione ESPAÇO para continuar." << std::endl;
        }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        // Reset do teste
        currentTriangleCount = 1;
        testRunning = false;
        testComplete = false;
        std::cout << "\nTeste resetado." << std::endl;
    }
}

// Inicializar GLFW e criar janela
bool initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW" << std::endl;
        return false;
    }

    // Configurar versão do OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criar janela
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
        
        if (testRunning) {
            std::cout << "Triângulos: " << currentTriangleCount 
                      << " | FPS: " << fps << std::endl;
            
            // Registrar dados de performance
            if (perfMonitor) {
                perfMonitor->addDataPoint(fps, currentTriangleCount);
            }
            
            // Avançar para próximo teste
            currentTriangleCount += triangleStep;
            if (currentTriangleCount > maxTriangles) {
                testComplete = true;
                testRunning = false;
                std::cout << "\n=== TESTE CONCLUÍDO ===" << std::endl;
                std::cout << "Gerando relatórios..." << std::endl;
                
                // Gerar relatórios
                if (perfMonitor) {
                    perfMonitor->generateCSVReport("data/performance_data.csv");
                    perfMonitor->generatePerformanceReport("reports/performance_report.txt");
                }
                
                std::cout << "Relatórios gerados! Execute 'python3 scripts/analyze_data.py data/performance_data.csv' para análise." << std::endl;
            } else {
                // Configurar próximo teste
                if (renderer) {
                    renderer->setTriangleCount(currentTriangleCount);
                }
            }
        } else {
            std::cout << "FPS: " << fps << " | Triângulos: " << currentTriangleCount << std::endl;
        }
        
        frameCount = 0;
        lastTime = currentTime;
    }
}

// Loop principal de renderização
void renderLoop() {
    double lastFrameTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        // Limpar buffer com fundo mais claro
        glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
        
        // Pequena pausa entre testes para estabilizar
        if (testRunning && !testComplete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main() {
    std::cout << "=== OpenGL Performance Test - Múltiplos Triângulos ===" << std::endl;
    std::cout << "Iniciando teste de desempenho..." << std::endl;

    // Criar diretórios necessários
    system("mkdir -p data reports");

    // Inicializar GLFW
    if (!initializeGLFW()) {
        return -1;
    }

    // Inicializar GLEW
    if (!initializeGLEW()) {
        glfwTerminate();
        return -1;
    }

    // Criar renderer
    renderer = new MultiTriangleRenderer();
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

    std::cout << "\nControles:" << std::endl;
    std::cout << "ESPAÇO - Iniciar/Pausar teste de performance" << std::endl;
    std::cout << "R - Reset do teste" << std::endl;
    std::cout << "ESC - Sair" << std::endl;
    std::cout << "\nO teste irá variar de 1 até " << maxTriangles 
              << " triângulos em passos de " << triangleStep << std::endl;
    std::cout << "Pressione ESPAÇO para iniciar..." << std::endl;

    // Loop principal
    renderLoop();

    // Limpeza
    delete renderer;
    delete perfMonitor;
    glfwTerminate();

    std::cout << "\nPrograma finalizado!" << std::endl;
    return 0;
}
