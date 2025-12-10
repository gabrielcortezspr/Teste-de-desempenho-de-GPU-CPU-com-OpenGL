#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include "AdvancedRenderer.h"
#include "PerformanceMonitor.h"

// Configurações da janela
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "OpenGL Performance Test - Iluminação e Texturas";

// Variáveis globais
GLFWwindow* window = nullptr;
AdvancedRenderer* renderer = nullptr;
PerformanceMonitor* perfMonitor = nullptr;

// Controle de FPS
double lastTime = 0.0;
int frameCount = 0;
double fps = 0.0;

// Configurações do teste
int currentTriangleCount = 1;
int maxTriangles = 500;  // Reduzido para testes com efeitos
int triangleStep = 25;
bool testRunning = false;
bool testComplete = false;

// Estados dos testes
enum TestState {
    BASIC_TEST,      // Sem efeitos
    LIGHTING_TEST,   // Com iluminação
    TEXTURE_TEST,    // Com texturas
    COMBINED_TEST    // Com ambos
};

TestState currentTest = BASIC_TEST;
std::string testNames[] = {"Básico", "Iluminação", "Texturas", "Combinado"};

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
            std::cout << "\n=== INICIANDO TESTE AVANÇADO DE PERFORMANCE ===" << std::endl;
            currentTriangleCount = 1;
            testComplete = false;
            currentTest = BASIC_TEST;
        } else {
            std::cout << "\nTeste pausado. Pressione ESPAÇO para continuar." << std::endl;
        }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        // Reset do teste
        currentTriangleCount = 1;
        testRunning = false;
        testComplete = false;
        currentTest = BASIC_TEST;
        std::cout << "\nTeste resetado." << std::endl;
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        // Alternar iluminação
        if (renderer) {
            renderer->setLightingEnabled(!renderer->isLightingEnabled());
        }
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        // Alternar texturas
        if (renderer) {
            renderer->setTexturesEnabled(!renderer->isTexturesEnabled());
        }
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
            bool hasLighting = renderer->isLightingEnabled();
            bool hasTextures = renderer->isTexturesEnabled();
            
            std::cout << "Teste: " << testNames[currentTest] 
                      << " | Triângulos: " << currentTriangleCount 
                      << " | FPS: " << fps << std::endl;
            
            // Registrar dados de performance
            if (perfMonitor) {
                perfMonitor->addDataPoint(fps, currentTriangleCount, hasLighting, hasTextures);
            }
            
            // Avançar para próximo teste
            currentTriangleCount += triangleStep;
            if (currentTriangleCount > maxTriangles) {
                // Avançar para próximo tipo de teste
                currentTriangleCount = 1;
                currentTest = static_cast<TestState>(currentTest + 1);
                
                if (currentTest > COMBINED_TEST) {
                    testComplete = true;
                    testRunning = false;
                    std::cout << "\n=== TESTE AVANÇADO CONCLUÍDO ===" << std::endl;
                    std::cout << "Gerando relatórios..." << std::endl;
                    
                    // Gerar relatórios
                    if (perfMonitor) {
                        perfMonitor->generateCSVReport("data/advanced_performance_data.csv");
                        perfMonitor->generatePerformanceReport("reports/advanced_performance_report.txt");
                    }
                    
                    std::cout << "Relatórios gerados! Execute 'python3 scripts/analyze_data.py data/advanced_performance_data.csv' para análise." << std::endl;
                } else {
                    // Configurar próximo teste
                    switch (currentTest) {
                        case BASIC_TEST:
                            renderer->setLightingEnabled(false);
                            renderer->setTexturesEnabled(false);
                            break;
                        case LIGHTING_TEST:
                            renderer->setLightingEnabled(true);
                            renderer->setTexturesEnabled(false);
                            break;
                        case TEXTURE_TEST:
                            renderer->setLightingEnabled(false);
                            renderer->setTexturesEnabled(true);
                            break;
                        case COMBINED_TEST:
                            renderer->setLightingEnabled(true);
                            renderer->setTexturesEnabled(true);
                            break;
                    }
                    std::cout << "\nIniciando teste: " << testNames[currentTest] << std::endl;
                }
            } else {
                // Configurar próximo teste
                if (renderer) {
                    renderer->setTriangleCount(currentTriangleCount);
                }
            }
        } else {
            std::cout << "FPS: " << fps << " | Triângulos: " << currentTriangleCount 
                      << " | Iluminação: " << (renderer->isLightingEnabled() ? "ON" : "OFF")
                      << " | Texturas: " << (renderer->isTexturesEnabled() ? "ON" : "OFF") << std::endl;
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
        
        // Pequena pausa entre testes para estabilizar
        if (testRunning && !testComplete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main() {
    std::cout << "=== OpenGL Performance Test - Iluminação e Texturas ===" << std::endl;
    std::cout << "Iniciando teste avançado de desempenho..." << std::endl;

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

    // Habilitar teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // Criar renderer avançado
    renderer = new AdvancedRenderer();
    if (!renderer->initialize()) {
        std::cerr << "Erro ao inicializar renderer avançado" << std::endl;
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
    std::cout << "L - Alternar iluminação" << std::endl;
    std::cout << "T - Alternar texturas" << std::endl;
    std::cout << "R - Reset do teste" << std::endl;
    std::cout << "ESC - Sair" << std::endl;
    std::cout << "\nO teste irá executar 4 fases:" << std::endl;
    std::cout << "1. Básico (sem efeitos)" << std::endl;
    std::cout << "2. Com iluminação" << std::endl;
    std::cout << "3. Com texturas" << std::endl;
    std::cout << "4. Combinado (iluminação + texturas)" << std::endl;
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
