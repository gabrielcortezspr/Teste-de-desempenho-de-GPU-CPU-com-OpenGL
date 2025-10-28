#ifndef MULTITRIANGLERENDERER_H
#define MULTITRIANGLERENDERER_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <random>

struct Triangle {
    float x, y, z;
    float r, g, b;
    float rotationSpeed;
    float currentRotation;
};

class MultiTriangleRenderer {
private:
    GLuint VAO, VBO;
    GLuint shaderProgram;
    std::vector<Triangle> triangles;
    std::mt19937 rng;
    
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
    
    // Gerar triângulos aleatórios
    void generateTriangles(int count);

public:
    MultiTriangleRenderer();
    ~MultiTriangleRenderer();
    
    bool initialize();
    void setTriangleCount(int count);
    void render(float deltaTime);
    void cleanup();
    
    int getTriangleCount() const { return triangles.size(); }
};

#endif
