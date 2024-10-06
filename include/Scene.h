#pragma once

#include "Camera.h"
#include "Shader.h"
#include "includes.h"

class Scene {
  public:
    Scene(unsigned int width, unsigned int height);

    // Initializes the scene
    bool initialize();

    // Runs the main loop
    void run();

    // Cleans up the resources
    void cleanup();

  private:
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;

    GLFWwindow *window;
    Shader     *shaderProgram;
    Camera     *camera;

    unsigned int VAO, VBO;
    unsigned int texture1, texture2;

    float deltaTime;
    float lastFrame;

    float lastX;
    float lastY;
    bool  firstMouse;

    // Process input
    void processInput();

    // GLFW Callbacks
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    // FPS counter
    void fps_counter();

    // Load textures
    bool loadTextures();

    // Create buffers
    void createBuffers();

    // Helper function to get the Scene instance from window's user pointer
    static Scene *getScene(GLFWwindow *window);
};
