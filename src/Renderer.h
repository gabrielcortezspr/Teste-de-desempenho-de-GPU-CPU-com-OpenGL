#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <vector>
#include <string>

class Renderer {
private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
    
    // Dados do triângulo
    float triangleVertices[18] = {
        // Posições (x, y, z) e Cores (r, g, b)
         0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Topo - Vermelho
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Esquerda - Verde
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Direita - Azul
    };

    // Shader source code
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        uniform mat4 transform;
        
        out vec3 vertexColor;
        
        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
            vertexColor = aColor;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )";

    // Compilar shader
    GLuint compileShader(GLenum type, const char* source);
    
    // Criar programa de shader
    bool createShaderProgram();

public:
    Renderer();
    ~Renderer();
    
    bool initialize();
    void renderTriangle(float rotationAngle);
    void cleanup();
};

#endif
