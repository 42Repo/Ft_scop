#pragma once

#include "struct.h"

class Scene;

class InputHandler {
  public:
    static void initialize(GLFWwindow *window, Scene *scene);

    static void processInput(float deltaTime);

  private:
    static GLFWwindow *_window;

    static Scene *_scene;

    static float _lastMouseX;
    static float _lastMouseY;

    // Mouse offsets
    static float _mouseDeltaX;
    static float _mouseDeltaY;

    static float _scrollOffset;

    static bool _firstMouse;

    static std::array<bool, 1024> _keys;

    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};
