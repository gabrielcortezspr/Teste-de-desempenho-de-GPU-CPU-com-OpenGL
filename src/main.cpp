#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>
#include "Renderer.h"
#include "PerformanceMonitor.h"

// Configurações da janela
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "OpenGL Performance Test - Triângulo Girando";

// Variáveis globais
GLFWwindow* window = nullptr;
Renderer* renderer = nullptr;
PerformanceMonitor* perfMonitor = nullptr;

// Controle de FPS
double lastTime = 0.0;
int frameCount = 0;
double fps = 0.0;

// Ângulo de rotação
float rotationAngle = 0.0f;

// Callback para redimensionamento da janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback para teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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
        std::cout << "FPS: " << fps << std::endl;
        
        frameCount = 0;
        lastTime = currentTime;
    }
}

// Loop principal de renderização
void renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        // Limpar buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Atualizar ângulo de rotação
        rotationAngle += 0.01f;
        if (rotationAngle > 2 * M_PI) {
            rotationAngle = 0.0f;
        }

        // Renderizar triângulo
        if (renderer) {
            renderer->renderTriangle(rotationAngle);
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
    std::cout << "=== OpenGL Performance Test ===" << std::endl;
    std::cout << "Iniciando teste de desempenho..." << std::endl;

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
    renderer = new Renderer();
    if (!renderer->initialize()) {
        std::cerr << "Erro ao inicializar renderer" << std::endl;
        delete renderer;
        glfwTerminate();
        return -1;
    }

    // Criar monitor de performance
    perfMonitor = new PerformanceMonitor();
    perfMonitor->initialize();

    std::cout << "\nControles:" << std::endl;
    std::cout << "ESC - Sair" << std::endl;
    std::cout << "Observe o FPS no console..." << std::endl;

    // Loop principal
    renderLoop();

    // Limpeza
    delete renderer;
    delete perfMonitor;
    glfwTerminate();

    std::cout << "\nTeste concluído!" << std::endl;
    return 0;
}
