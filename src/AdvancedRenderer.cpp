#include "AdvancedRenderer.h"
#include <iostream>
#include <cmath>

AdvancedRenderer::AdvancedRenderer() : VAO(0), VBO(0), lighting(nullptr), texture(nullptr), 
                                       useLighting(false), useTextures(false), rng(std::random_device{}()) {
}

AdvancedRenderer::~AdvancedRenderer() {
    cleanup();
}

bool AdvancedRenderer::initialize() {
    // Criar sistema de iluminação
    lighting = new Lighting();
    if (!lighting->initialize()) {
        std::cerr << "Erro ao inicializar sistema de iluminação" << std::endl;
        return false;
    }
    
    // Criar textura
    texture = new Texture();
    if (!texture->generateProcedural()) {
        std::cerr << "Erro ao gerar textura" << std::endl;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de cor (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Atributo de normal (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Atributo de coordenada de textura (location = 3)
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // Desvincular VAO
    glBindVertexArray(0);

    std::cout << "AdvancedRenderer inicializado com sucesso!" << std::endl;
    return true;
}

void AdvancedRenderer::setTriangleCount(int count) {
    if (count <= 0) return;
    
    generateTriangles(count);
    
    // Atualizar buffer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Criar dados de vértices para todos os triângulos
    std::vector<float> vertices;
    vertices.reserve(count * 33); // 11 floats por vértice, 3 vértices por triângulo
    
    for (const auto& triangle : triangles) {
        // Vértice 1 (topo)
        vertices.push_back(triangle.x);
        vertices.push_back(triangle.y + 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
        vertices.push_back(triangle.nx);
        vertices.push_back(triangle.ny);
        vertices.push_back(triangle.nz);
        vertices.push_back(0.5f);
        vertices.push_back(1.0f);
        
        // Vértice 2 (esquerda)
        vertices.push_back(triangle.x - 0.1f);
        vertices.push_back(triangle.y - 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
        vertices.push_back(triangle.nx);
        vertices.push_back(triangle.ny);
        vertices.push_back(triangle.nz);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        
        // Vértice 3 (direita)
        vertices.push_back(triangle.x + 0.1f);
        vertices.push_back(triangle.y - 0.1f);
        vertices.push_back(triangle.z);
        vertices.push_back(triangle.r);
        vertices.push_back(triangle.g);
        vertices.push_back(triangle.b);
        vertices.push_back(triangle.nx);
        vertices.push_back(triangle.ny);
        vertices.push_back(triangle.nz);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
    }
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    std::cout << "Triângulos avançados configurados: " << count << std::endl;
}

void AdvancedRenderer::setLightingEnabled(bool enabled) {
    useLighting = enabled;
    std::cout << "Iluminação " << (enabled ? "habilitada" : "desabilitada") << std::endl;
}

void AdvancedRenderer::setTexturesEnabled(bool enabled) {
    useTextures = enabled;
    std::cout << "Texturas " << (enabled ? "habilitadas" : "desabilitadas") << std::endl;
}

void AdvancedRenderer::render(float deltaTime) {
    glBindVertexArray(VAO);
    
    if (useLighting) {
        lighting->useShader();
        lighting->setViewPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        
        // Configurar matrizes
        GLint modelLoc = glGetUniformLocation(lighting->getShaderProgram(), "model");
        GLint viewLoc = glGetUniformLocation(lighting->getShaderProgram(), "view");
        GLint projLoc = glGetUniformLocation(lighting->getShaderProgram(), "projection");
        
        // Matriz de visualização
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
                                   glm::vec3(0.0f, 0.0f, 0.0f), 
                                   glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        // Matriz de projeção
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Configurar textura
        GLint useTextureLoc = glGetUniformLocation(lighting->getShaderProgram(), "useTexture");
        glUniform1i(useTextureLoc, useTextures);
        
        if (useTextures) {
            texture->bind(GL_TEXTURE0);
        }
        
        // Renderizar cada triângulo com iluminação
        for (size_t i = 0; i < triangles.size(); ++i) {
            AdvancedTriangle& triangle = triangles[i];
            
            // Atualizar rotação
            triangle.currentRotation += triangle.rotationSpeed * deltaTime;
            if (triangle.currentRotation > 2 * M_PI) {
                triangle.currentRotation = 0.0f;
            }
            
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(triangle.x, triangle.y, triangle.z));
            transform = glm::rotate(transform, triangle.currentRotation, glm::vec3(0.0f, 0.0f, 1.0f));
            
            GLint transformLoc = glGetUniformLocation(lighting->getShaderProgram(), "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            GLint modelLoc = glGetUniformLocation(lighting->getShaderProgram(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            // Desenhar triângulo
            glDrawArrays(GL_TRIANGLES, i * 3, 3);
        }
    } else {
        // Renderização simples sem iluminação (similar ao MultiTriangleRenderer)
        // Criar um shader simples se não tiver iluminação
        // Por enquanto, vamos usar o shader de iluminação mesmo assim, mas sem as luzes ativas
        lighting->useShader();
        
        // Configurar para não usar texturas nem iluminação
        GLint useTextureLoc = glGetUniformLocation(lighting->getShaderProgram(), "useTexture");
        glUniform1i(useTextureLoc, 0);
        
        GLint numLightsLoc = glGetUniformLocation(lighting->getShaderProgram(), "numLights");
        glUniform1i(numLightsLoc, 0); // Sem luzes
        
        // Matrizes de visualização
        GLint viewLoc = glGetUniformLocation(lighting->getShaderProgram(), "view");
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
                                   glm::vec3(0.0f, 0.0f, 0.0f), 
                                   glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        GLint projLoc = glGetUniformLocation(lighting->getShaderProgram(), "projection");
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1024.0f/768.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Renderizar cada triângulo
        for (size_t i = 0; i < triangles.size(); ++i) {
            AdvancedTriangle& triangle = triangles[i];
            
            // Atualizar rotação
            triangle.currentRotation += triangle.rotationSpeed * deltaTime;
            if (triangle.currentRotation > 2 * M_PI) {
                triangle.currentRotation = 0.0f;
            }
            
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(triangle.x, triangle.y, triangle.z));
            transform = glm::rotate(transform, triangle.currentRotation, glm::vec3(0.0f, 0.0f, 1.0f));
            
            GLint transformLoc = glGetUniformLocation(lighting->getShaderProgram(), "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            GLint modelLoc = glGetUniformLocation(lighting->getShaderProgram(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
            
            // Desenhar triângulo
            glDrawArrays(GL_TRIANGLES, i * 3, 3);
        }
    }
    
    glBindVertexArray(0);
}

void AdvancedRenderer::generateTriangles(int count) {
    triangles.clear();
    triangles.reserve(count);
    
    std::uniform_real_distribution<float> posDist(-0.8f, 0.8f);
    std::uniform_real_distribution<float> colorDist(0.5f, 1.0f);
    std::uniform_real_distribution<float> speedDist(0.5f, 2.0f);
    
    for (int i = 0; i < count; ++i) {
        AdvancedTriangle triangle;
        triangle.x = posDist(rng);
        triangle.y = posDist(rng);
        triangle.z = 0.0f;
        triangle.r = colorDist(rng);
        triangle.g = colorDist(rng);
        triangle.b = colorDist(rng);
        triangle.nx = 0.0f;
        triangle.ny = 0.0f;
        triangle.nz = 1.0f;
        triangle.u = 0.5f;
        triangle.v = 0.5f;
        triangle.rotationSpeed = speedDist(rng);
        triangle.currentRotation = 0.0f;
        
        triangles.push_back(triangle);
    }
}

void AdvancedRenderer::cleanup() {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (lighting) {
        delete lighting;
        lighting = nullptr;
    }
    if (texture) {
        delete texture;
        texture = nullptr;
    }
}
