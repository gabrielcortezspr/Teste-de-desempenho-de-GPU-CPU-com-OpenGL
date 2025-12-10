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
const char* WINDOW_TITLE = "OpenGL Extreme Performance Test";

// Variáveis globais
GLFWwindow* window = nullptr;
AdvancedRenderer* renderer = nullptr;
PerformanceMonitor* perfMonitor = nullptr;

// Controle de FPS
double lastTime = 0.0;
int frameCount = 0;
double fps = 0.0;

// Configurações do teste EXTREMO
int currentTriangleCount = 10000;
int maxTriangles = 200000;  // 200 mil triângulos!
int triangleStep = 10000;   // Passo de 10 mil
bool testRunning = false;
bool testComplete = false;

// Estados dos testes (3 modos)
enum TestState {
    BASIC_TEST,
    LIGHTING_TEST,
    COMBINED_TEST
};

TestState currentTest = BASIC_TEST;
std::string testNames[] = {"Básico", "Iluminação", "Combinado"};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (!testRunning && !testComplete) {
            testRunning = true;
            std::cout << "\n=== INICIANDO TESTE EXTREMO AUTOMÁTICO ===" << std::endl;
            std::cout << "PASSOS DE 10.000 TRIÂNGULOS ATÉ 200.000!" << std::endl;
            std::cout << "O teste rodará automaticamente pelos 3 modos" << std::endl;
            currentTriangleCount = 10000;
            testComplete = false;
            currentTest = BASIC_TEST;
            
            // Configurar modo inicial (básico)
            if (renderer) {
                renderer->setLightingEnabled(false);
                renderer->setTexturesEnabled(false);
                renderer->setTriangleCount(currentTriangleCount);
            }
            std::cout << "\n>>> Teste 1/3: BÁSICO iniciado..." << std::endl;
        }
    }
}

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

void updateFPS() {
    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 2.0) { // 2 segundos por medição (testes pesados)
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
                currentTriangleCount = 10000;
                currentTest = static_cast<TestState>(currentTest + 1);
                
                if (currentTest > COMBINED_TEST) {
                    testComplete = true;
                    testRunning = false;
                    std::cout << "\n=== TESTE EXTREMO CONCLUÍDO ===" << std::endl;
                    std::cout << "Gerando relatórios..." << std::endl;
                    
                    // Gerar relatórios
                    if (perfMonitor) {
                        perfMonitor->generateCSVReport("data/extreme_performance_data.csv");
                        perfMonitor->generatePerformanceReport("reports/extreme_performance_report.txt");
                    }
                    
                    std::cout << "Relatórios gerados em data/extreme_performance_data.csv" << std::endl;
                    std::cout << "Execute: python3 scripts/analyze_data.py data/extreme_performance_data.csv" << std::endl;
                } else {
                    // Configurar próximo teste
                    switch (currentTest) {
                        case BASIC_TEST:
                            std::cout << "\n>>> Teste 1/3: BÁSICO concluído!" << std::endl;
                            std::cout << ">>> Teste 2/3: ILUMINAÇÃO iniciado..." << std::endl;
                            renderer->setLightingEnabled(true);
                            renderer->setTexturesEnabled(false);
                            break;
                        case LIGHTING_TEST:
                            std::cout << "\n>>> Teste 2/3: ILUMINAÇÃO concluído!" << std::endl;
                            std::cout << ">>> Teste 3/3: COMBINADO (Iluminação + Texturas) iniciado..." << std::endl;
                            renderer->setLightingEnabled(true);
                            renderer->setTexturesEnabled(true);
                            break;
                        case COMBINED_TEST:
                            // Não deveria chegar aqui
                            break;
                    }
                    // Configurar os triângulos para o novo teste
                    if (renderer) {
                        renderer->setTriangleCount(currentTriangleCount);
                    }
                }
            } else {
                // Aumentar número de triângulos
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

void renderLoop() {
    double lastFrameTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (renderer) {
            renderer->render(deltaTime);
        }

        if (perfMonitor) {
            perfMonitor->update();
        }

        updateFPS();

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        if (testRunning && !testComplete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

int main() {
    std::cout << "=== TESTE EXTREMO DE PERFORMANCE ===" << std::endl;
    std::cout << "Este teste usa passos de 10.000 triângulos!" << std::endl;
    std::cout << "Variando de 10.000 até 200.000 triângulos" << std::endl;
    std::cout << "3 Modos: Básico | Iluminação | Combinado (Iluminação + Texturas)" << std::endl;
    std::cout << "⚠️  AVISO: Este teste é PESADO e pode deixar o sistema lento!\n" << std::endl;

    system("mkdir -p data reports");

    if (!initializeGLFW()) {
        return -1;
    }

    if (!initializeGLEW()) {
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    renderer = new AdvancedRenderer();
    if (!renderer->initialize()) {
        std::cerr << "Erro ao inicializar renderer" << std::endl;
        delete renderer;
        glfwTerminate();
        return -1;
    }

    renderer->setTriangleCount(currentTriangleCount);

    perfMonitor = new PerformanceMonitor();
    perfMonitor->initialize();

    std::cout << "\nControles:" << std::endl;
    std::cout << "ESPAÇO - Iniciar teste extremo automático" << std::endl;
    std::cout << "ESC - Sair" << std::endl;
    std::cout << "\nPressione ESPAÇO para iniciar...\n" << std::endl;

    renderLoop();

    delete renderer;
    delete perfMonitor;
    glfwTerminate();

    std::cout << "\nTeste extremo finalizado!" << std::endl;
    return 0;
}
