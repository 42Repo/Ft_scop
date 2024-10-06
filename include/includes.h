#pragma once

#include "glad/glad.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

void        framebuffer_size_callback(GLFWwindow *window, int width, int height);
void        processInput(GLFWwindow *window);
GLFWwindow *initGLFW();
bool        initGLAD();
void        createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void        renderLoop(GLFWwindow *window, class Shader &shader, unsigned int VAO, unsigned int EBO,
                       unsigned int texture1, unsigned int texture2);
