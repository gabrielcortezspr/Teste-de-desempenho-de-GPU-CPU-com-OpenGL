#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
private:
    GLuint textureID;
    int width, height, nrChannels;
    std::string path;
    
    // Gerar textura procedural
    GLubyte* generateProceduralTexture(int width, int height);

public:
    Texture();
    ~Texture();
    
    bool loadFromFile(const std::string& filePath);
    bool generateProcedural(int width = 256, int height = 256);
    void bind(GLenum textureUnit = GL_TEXTURE0);
    void cleanup();
    
    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif
