#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
    bool enabled;
    
    Light() : position(0.0f), color(1.0f), intensity(1.0f), radius(10.0f), enabled(true) {}
};

class Lighting {
private:
    GLuint shaderProgram;
    std::vector<Light> lights;
    
    // Shader source code com iluminação
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec3 aNormal;
        layout (location = 3) in vec2 aTexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform mat4 transform;
        
        out vec3 FragPos;
        out vec3 VertexColor;
        out vec3 Normal;
        out vec2 TexCoord;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            VertexColor = aColor;
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoord = aTexCoord;
            
            gl_Position = projection * view * transform * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        struct Light {
            vec3 position;
            vec3 color;
            float intensity;
            float radius;
            bool enabled;
        };
        
        in vec3 FragPos;
        in vec3 VertexColor;
        in vec3 Normal;
        in vec2 TexCoord;
        
        uniform vec3 viewPos;
        uniform Light lights[8];
        uniform int numLights;
        uniform sampler2D texture1;
        uniform bool useTexture;
        
        out vec4 FragColor;
        
        vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
            if (!light.enabled) return vec3(0.0);
            
            vec3 lightDir = normalize(light.position - fragPos);
            float distance = length(light.position - fragPos);
            
            // Atenuação
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
            
            // Difusa
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * light.color * light.intensity * attenuation;
            
            // Especular
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * light.color * light.intensity * attenuation;
            
            return diffuse + specular;
        }
        void main() {
            vec3 normal = normalize(Normal);
            vec3 viewDir = normalize(viewPos - FragPos);
            
            // Usar cor do vértice como base
            vec3 baseColor = VertexColor;
            
            // Luz ambiente mais forte
            vec3 ambient = vec3(0.6, 0.6, 0.7);
            vec3 result = ambient * baseColor;
            
            // Calcular todas as luzes
            for (int i = 0; i < numLights; i++) {
                result += calculateLight(lights[i], normal, FragPos, viewDir) * baseColor;
            }
            
            if (useTexture) {
                vec4 texColor = texture(texture1, TexCoord);
                result *= texColor.rgb;
            }
            
            FragColor = vec4(result, 1.0);
        }
    )";

    // Compilar shader
    GLuint compileShader(GLenum type, const char* source);
    
    // Criar programa de shader
    bool createShaderProgram();

public:
    Lighting();
    ~Lighting();
    
    bool initialize();
    void addOmnidirectionalLight(glm::vec3 position, glm::vec3 color, float intensity);
    void addSpotLight(glm::vec3 position, glm::vec3 color, float intensity, float radius);
    void setViewPosition(glm::vec3 viewPos);
    void useShader();
    void cleanup();
    
    GLuint getShaderProgram() const { return shaderProgram; }
    int getLightCount() const { return lights.size(); }
};

#endif
