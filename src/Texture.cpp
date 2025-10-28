#include "Texture.h"
#include <iostream>
#include <cmath>

Texture::Texture() : textureID(0), width(0), height(0), nrChannels(0) {
}

Texture::~Texture() {
    cleanup();
}

bool Texture::loadFromFile(const std::string& filePath) {
    path = filePath;
    
    // Por simplicidade, vamos gerar uma textura procedural
    // Em um projeto real, você usaria stb_image.h para carregar arquivos
    return generateProcedural();
}

bool Texture::generateProcedural(int width, int height) {
    this->width = width;
    this->height = height;
    this->nrChannels = 3;
    
    GLubyte* data = generateProceduralTexture(width, height);
    if (!data) {
        return false;
    }
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Configurar parâmetros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Carregar dados da textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    delete[] data;
    
    std::cout << "Textura procedural gerada: " << width << "x" << height << std::endl;
    return true;
}

GLubyte* Texture::generateProceduralTexture(int width, int height) {
    GLubyte* data = new GLubyte[width * height * 3];
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            
            // Criar padrão de xadrez colorido
            float u = (float)x / width;
            float v = (float)y / height;
            
            // Padrão de xadrez
            int checkerX = (int)(u * 8) % 2;
            int checkerY = (int)(v * 8) % 2;
            bool isWhite = (checkerX + checkerY) % 2 == 0;
            
            if (isWhite) {
                // Gradiente colorido
                data[index] = (GLubyte)(255 * (0.5f + 0.5f * sin(u * 3.14159f)));     // R
                data[index + 1] = (GLubyte)(255 * (0.5f + 0.5f * sin(v * 3.14159f))); // G
                data[index + 2] = (GLubyte)(255 * (0.5f + 0.5f * sin((u + v) * 3.14159f))); // B
            } else {
                // Preto
                data[index] = 0;
                data[index + 1] = 0;
                data[index + 2] = 0;
            }
        }
    }
    
    return data;
}

void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::cleanup() {
    if (textureID) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}
