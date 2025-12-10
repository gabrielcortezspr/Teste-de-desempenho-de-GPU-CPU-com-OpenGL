#ifndef ADVANCEDRENDERER_H
#define ADVANCEDRENDERER_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include <random>
#include "Lighting.h"
#include "Texture.h"

struct AdvancedTriangle {
    float x, y, z;
    float r, g, b;     // Cor
    float nx, ny, nz;  // Normais
    float u, v;        // Coordenadas de textura
    float rotationSpeed;
    float currentRotation;
};

class AdvancedRenderer {
private:
    GLuint VAO, VBO;
    Lighting* lighting;
    Texture* texture;
    std::vector<AdvancedTriangle> triangles;
    std::mt19937 rng;
    
    bool useLighting;
    bool useTextures;
    
    // Dados do triângulo com normais e coordenadas de textura
    float triangleVertices[24] = {
        // Posições (x, y, z), Normais (nx, ny, nz), Coordenadas de textura (u, v)
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.0f,  // Topo
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // Esquerda
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f   // Direita
    };

    // Gerar triângulos aleatórios
    void generateTriangles(int count);

public:
    AdvancedRenderer();
    ~AdvancedRenderer();
    
    bool initialize();
    void setTriangleCount(int count);
    void setLightingEnabled(bool enabled);
    void setTexturesEnabled(bool enabled);
    void render(float deltaTime);
    void cleanup();
    
    int getTriangleCount() const { return triangles.size(); }
    bool isLightingEnabled() const { return useLighting; }
    bool isTexturesEnabled() const { return useTextures; }
};

#endif
