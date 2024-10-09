#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void        framebuffer_size_callback(GLFWwindow *window, int width, int height);
GLFWwindow *initGLFW();
bool        initGLAD();
void        createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void        mouse_callback(GLFWwindow *window, double xpos, double ypos);
void        processInput(GLFWwindow *window);
void        scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void        renderLoop(GLFWwindow *window, class Shader &shader, Camera &camera, Mesh &mesh,
                       Texture &texture1, Texture &texture2);
