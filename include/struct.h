#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Shader;
class Camera;
class Mesh;
class Texture;

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
};

struct SubMesh {
    std::vector<unsigned int> indices;
    std::string               materialName;
};

struct ObjObject {
    std::string          name;
    std::vector<Vertex>  vertices;
    std::vector<SubMesh> subMeshes;
};

struct Material {
    std::string                      name;
    glm::vec3                        ambient = glm::vec3(0.0f);
    glm::vec3                        diffuse = glm::vec3(0.0f);
    glm::vec3                        specular = glm::vec3(0.0f);
    float                            shininess = 32.0f;
    std::string                      diffuseMapPath;
    mutable std::shared_ptr<Texture> diffuseTexture;
};

void        framebuffer_size_callback(GLFWwindow *window, int width, int height);
GLFWwindow *initGLFW();
bool        initGLAD();
void        createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void        mouse_callback(GLFWwindow *window, double xpos, double ypos);
void        processInput(GLFWwindow *window);
void        scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void        renderLoop(GLFWwindow *window, class Shader &shader, Camera &camera, Mesh &mesh,
                       Texture &texture1, Texture &texture2);
