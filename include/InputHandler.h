#pragma once

#include "struct.h"

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

    // Boolean array to keep track of key presses
    static std::array<bool, 1024> _keys;

    // Callback functionsu
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};
