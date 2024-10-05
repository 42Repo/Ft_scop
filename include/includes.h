#pragma once

// #include <GLFW/glfw3.h>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

void        framebuffer_size_callback(GLFWwindow *window, int width, int height);
void        processInput(GLFWwindow *window);
GLFWwindow *initGLFW();
bool        initGLAD();
void        createBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void renderLoop(GLFWwindow *window, class Shader &shader, unsigned int VAO, unsigned int EBO);
