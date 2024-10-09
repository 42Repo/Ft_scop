#pragma once

#include <GLFW/glfw3.h>

class Scene;

class InputHandler {
  public:
    // Initialize the input handler with a GLFW window and a camera reference
    static void initialize(GLFWwindow *window, Scene *scene);

    // Process input (to be called every frame)
    static void processInput(float deltaTime);

  private:
    // GLFW window
    static GLFWwindow *_window;

    // Scene reference
    static Scene *_scene;

    // Last mouse positions
    static float _lastMouseX;
    static float _lastMouseY;

    // Mouse offsets
    static float _mouseDeltaX;
    static float _mouseDeltaY;

    // Scroll offset
    static float _scrollOffset;

    // First mouse movement flag
    static bool _firstMouse;

    // Boolean flags for key presses
    static bool _keys[1024];

    // Callback functionsu
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};
