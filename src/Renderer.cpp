#include "Renderer.h"
#include <iostream>
#include <cmath>

Renderer::Renderer() : VAO(0), VBO(0), shaderProgram(0) {
}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize() {
    // Criar programa de shader
    if (!createShaderProgram()) {
        return false;
    }

    // Gerar e vincular VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Gerar e vincular VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // Configurar atributos de vértice
    // Atributo de posição (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de cor (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desvincular VAO
    glBindVertexArray(0);

    std::cout << "Renderer inicializado com sucesso!" << std::endl;
    return true;
}

void Renderer::renderTriangle(float rotationAngle) {
    // Usar programa de shader
    glUseProgram(shaderProgram);

    // Criar matriz de transformação (rotação)
    float cosA = cos(rotationAngle);
    float sinA = sin(rotationAngle);
    
    float transformMatrix[16] = {
        cosA, -sinA, 0.0f, 0.0f,
        sinA,  cosA, 0.0f, 0.0f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f
    };

    // Enviar matriz de transformação para o shader
    GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transformMatrix);

    // Vincular VAO e desenhar
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Renderer::cleanup() {
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

GLuint Renderer::compileShader(GLenum type, const char* source) {
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

bool Renderer::createShaderProgram() {
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

    std::cout << "Programa de shader criado com sucesso!" << std::endl;
    return true;
}
