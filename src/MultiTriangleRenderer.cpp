#include "MultiTriangleRenderer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MultiTriangleRenderer::MultiTriangleRenderer() : VAO(0), VBO(0), shaderProgram(0), rng(std::random_device{}()) {
}

MultiTriangleRenderer::~MultiTriangleRenderer() {
    cleanup();
}

bool MultiTriangleRenderer::initialize() {
    // Criar programa de shader
    if (!createShaderProgram()) {
        return false;
    }

    // Gerar triângulos iniciais
    generateTriangles(1);

    // Gerar e vincular VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Gerar e vincular VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Configurar atributos de vértice
    // Atributo de posição (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de cor (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desvincular VAO
    glBindVertexArray(0);

    std::cout << "MultiTriangleRenderer inicializado com sucesso!" << std::endl;
    return true;
}

void MultiTriangleRenderer::setTriangleCount(int count) {
    if (count <= 0) return;
    
    generateTriangles(count);
    
    // Atualizar buffer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Criar dados de vértices para todos os triângulos
    std::vector<float> vertices;
    vertices.reserve(count * 18); // 6 floats por vértice, 3 vértices por triângulo
    
    for (const auto& triangle : triangles) {
        // Vértice 1 (topo)
        vertices.push_back(triangle.x);
        vertices.push_back(triangle.y + 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
        
        // Vértice 2 (esquerda)
        vertices.push_back(triangle.x - 0.1f);
        vertices.push_back(triangle.y - 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
        
        // Vértice 3 (direita)
        vertices.push_back(triangle.x + 0.1f);
        vertices.push_back(triangle.y - 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
    }
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    std::cout << "Triângulos configurados: " << count << std::endl;
}

void MultiTriangleRenderer::render(float deltaTime) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    
    // Renderizar cada triângulo com sua própria rotação
    for (size_t i = 0; i < triangles.size(); ++i) {
        Triangle& triangle = triangles[i];
        
        // Atualizar rotação
        triangle.currentRotation += triangle.rotationSpeed * deltaTime;
        if (triangle.currentRotation > 2 * M_PI) {
            triangle.currentRotation = 0.0f;
        }
        
        // Criar matriz de transformação
        float cosA = cos(triangle.currentRotation);
        float sinA = sin(triangle.currentRotation);
        
        float transformMatrix[16] = {
            cosA, -sinA, 0.0f, triangle.x,
            sinA,  cosA, 0.0f, triangle.y,
            0.0f,  0.0f, 1.0f, triangle.z,
            0.0f,  0.0f, 0.0f, 1.0f
        };
        
        // Enviar matriz de transformação para o shader
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transformMatrix);
        
        // Desenhar triângulo
        glDrawArrays(GL_TRIANGLES, i * 3, 3);
    }
    
    glBindVertexArray(0);
}

void MultiTriangleRenderer::generateTriangles(int count) {
    triangles.clear();
    triangles.reserve(count);
    
    std::uniform_real_distribution<float> posDist(-0.8f, 0.8f);
    std::uniform_real_distribution<float> colorDist(0.5f, 1.0f);
    std::uniform_real_distribution<float> speedDist(0.5f, 2.0f);
    
    for (int i = 0; i < count; ++i) {
        Triangle triangle;
        triangle.x = posDist(rng);
        triangle.y = posDist(rng);
        triangle.z = 0.0f;
        triangle.r = colorDist(rng);
        triangle.g = colorDist(rng);
        triangle.b = colorDist(rng);
        triangle.rotationSpeed = speedDist(rng);
        triangle.currentRotation = 0.0f;
        
        triangles.push_back(triangle);
    }
}

void MultiTriangleRenderer::cleanup() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}

GLuint MultiTriangleRenderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Verificar erros de compilação
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erro ao compilar shader: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool MultiTriangleRenderer::createShaderProgram() {
    // Compilar shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (!vertexShader || !fragmentShader) {
        return false;
    }

    // Criar programa
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verificar erros de linking
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Erro ao linkar programa de shader: " << infoLog << std::endl;
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    // Limpar shaders individuais
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}
