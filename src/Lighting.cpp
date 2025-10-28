#include "Lighting.h"
#include <iostream>

Lighting::Lighting() : shaderProgram(0) {
}

Lighting::~Lighting() {
    cleanup();
}

bool Lighting::initialize() {
    if (!createShaderProgram()) {
        return false;
    }
    
    // Adicionar luzes padrão
    addOmnidirectionalLight(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    addSpotLight(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f), 1.5f, 5.0f);
    
    std::cout << "Sistema de iluminação inicializado com " << lights.size() << " luzes" << std::endl;
    return true;
}

void Lighting::addOmnidirectionalLight(glm::vec3 position, glm::vec3 color, float intensity) {
    Light light;
    light.position = position;
    light.color = color;
    light.intensity = intensity;
    light.radius = 10.0f;
    light.enabled = true;
    
    lights.push_back(light);
    std::cout << "Luz omnidirecional adicionada na posição (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void Lighting::addSpotLight(glm::vec3 position, glm::vec3 color, float intensity, float radius) {
    Light light;
    light.position = position;
    light.color = color;
    light.intensity = intensity;
    light.radius = radius;
    light.enabled = true;
    
    lights.push_back(light);
    std::cout << "Spotlight adicionada na posição (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
}

void Lighting::setViewPosition(glm::vec3 viewPos) {
    glUseProgram(shaderProgram);
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
}

void Lighting::useShader() {
    glUseProgram(shaderProgram);
    
    // Enviar luzes para o shader
    GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numLights");
    glUniform1i(numLightsLoc, lights.size());
    
    for (size_t i = 0; i < lights.size() && i < 8; ++i) {
        std::string lightName = "lights[" + std::to_string(i) + "]";
        
        GLint posLoc = glGetUniformLocation(shaderProgram, (lightName + ".position").c_str());
        GLint colorLoc = glGetUniformLocation(shaderProgram, (lightName + ".color").c_str());
        GLint intensityLoc = glGetUniformLocation(shaderProgram, (lightName + ".intensity").c_str());
        GLint radiusLoc = glGetUniformLocation(shaderProgram, (lightName + ".radius").c_str());
        GLint enabledLoc = glGetUniformLocation(shaderProgram, (lightName + ".enabled").c_str());
        
        if (posLoc != -1) glUniform3fv(posLoc, 1, glm::value_ptr(lights[i].position));
        if (colorLoc != -1) glUniform3fv(colorLoc, 1, glm::value_ptr(lights[i].color));
        if (intensityLoc != -1) glUniform1f(intensityLoc, lights[i].intensity);
        if (radiusLoc != -1) glUniform1f(radiusLoc, lights[i].radius);
        if (enabledLoc != -1) glUniform1i(enabledLoc, lights[i].enabled);
    }
}

void Lighting::cleanup() {
    if (shaderProgram) {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}

GLuint Lighting::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Verificar erros de compilação
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erro ao compilar shader de iluminação: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Lighting::createShaderProgram() {
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
        std::cerr << "Erro ao linkar programa de shader de iluminação: " << infoLog << std::endl;
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    // Limpar shaders individuais
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Programa de shader de iluminação criado com sucesso!" << std::endl;
    return true;
}
