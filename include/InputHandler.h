#pragma once

#include <GLFW/glfw3.h>

class Camera;

class InputHandler {
  public:
    // Initialize the input handler with a GLFW window and a camera reference
    static void initialize(GLFWwindow *window, Camera *camera);

    // Process input (to be called every frame)
    static void processInput(float deltaTime);

    // Get mouse positions and offsets
    static float getMouseX();
    static float getMouseY();
    static float getMouseDeltaX();
    static float getMouseDeltaY();

    // Get scroll offset
    static float getScrollOffset();

  private:
    // GLFW window
    static GLFWwindow *_window;

    // Camera reference
    static Camera *_camera;

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

    // Callback functionsu
    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
};
