#include "../include/InputHandler.h"
#include "../include/Camera.h"

const float EPSILON = 1e-6f;

// Initialize static members
GLFWwindow *InputHandler::_window = nullptr;
Camera     *InputHandler::_camera = nullptr;
float       InputHandler::_lastMouseX = 0.0f;
float       InputHandler::_lastMouseY = 0.0f;
float       InputHandler::_mouseDeltaX = 0.0f;
float       InputHandler::_mouseDeltaY = 0.0f;
float       InputHandler::_scrollOffset = 0.0f;
bool        InputHandler::_firstMouse = true;

void InputHandler::initialize(GLFWwindow *win, Camera *cam) {
    _window = win;
    _camera = cam;

    // Set initial mouse position to center of the screen
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    _lastMouseX = static_cast<float>(width) / 2.0f;
    _lastMouseY = static_cast<float>(height) / 2.0f;

    // Set GLFW callbacks
    glfwSetCursorPosCallback(_window, mouseCallback);
    glfwSetScrollCallback(_window, scrollCallback);

    // Capture the mouse
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputHandler::processInput(float deltaTime) {
    if (!_window || !_camera)
        return;

    // Close _window on ESCAPE key press
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    // Movement keys
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        _camera->processKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        _camera->processKeyboard(DOWNWARD, deltaTime);

    // Mouse movement (handled in mouseCallback)
    if (fabsf(_mouseDeltaX) > EPSILON || fabsf(_mouseDeltaY) > EPSILON) {
        _camera->processMouseMovement(_mouseDeltaX, _mouseDeltaY);
        _mouseDeltaX = 0.0f;
        _mouseDeltaY = 0.0f;
    }

    // Scroll input (handled in scrollCallback)
    if (fabsf(_scrollOffset) > EPSILON) {
        _camera->processMouseScroll(_scrollOffset);
        _scrollOffset = 0.0f;
    }
}

void InputHandler::mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    (void)window;
    if (_firstMouse) {
        _lastMouseX = static_cast<float>(xpos);
        _lastMouseY = static_cast<float>(ypos);
        _firstMouse = false;
    }

    _mouseDeltaX = static_cast<float>(xpos) - _lastMouseX;
    _mouseDeltaY = _lastMouseY -
                   static_cast<float>(ypos); // Reversed since y-coordinates go from bottom to top

    _lastMouseX = static_cast<float>(xpos);
    _lastMouseY = static_cast<float>(ypos);
}

void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    (void)window;
    (void)xoffset;
    _scrollOffset = static_cast<float>(yoffset);
}
