#include "../include/InputHandler.h"
#include "../include/Camera.h"
#include "../include/Scene.h"

const float EPSILON = 1e-6f;

// Initialize static members
GLFWwindow            *InputHandler::_window = nullptr;
Scene                 *InputHandler::_scene = nullptr;
float                  InputHandler::_lastMouseX = 0.0f;
float                  InputHandler::_lastMouseY = 0.0f;
float                  InputHandler::_mouseDeltaX = 0.0f;
float                  InputHandler::_mouseDeltaY = 0.0f;
float                  InputHandler::_scrollOffset = 0.0f;
bool                   InputHandler::_firstMouse = true;
std::array<bool, 1024> InputHandler::_keys;

void InputHandler::initialize(GLFWwindow *win, Scene *scene) {
    _window = win;
    _scene = scene;

    // Set initial mouse position to center of the screen
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    _lastMouseX = static_cast<float>(width) / 2.0f;
    _lastMouseY = static_cast<float>(height) / 2.0f;

    // Set GLFW callbacks
    glfwSetCursorPosCallback(_window, mouseCallback);
    glfwSetScrollCallback(_window, scrollCallback);

    // Disable cursor
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputHandler::processInput(float deltaTime) {
    if (!_window || !_scene)
        return;

    // Close _window on ESCAPE key press
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    auto camera = _scene->getActiveCamera();
    if (!camera)
        return;

    // Handle camera switching with keys (e.g., keys 1 and 2 for next/previous camera)
    if (glfwGetKey(_window, GLFW_KEY_1) == GLFW_PRESS) {
        if (!_keys.at(GLFW_KEY_1)) {
            _scene->nextCamera();
            _keys.at(GLFW_KEY_1) = true;
        }
    } else {
        _keys.at(GLFW_KEY_1) = false;
    }

    if (glfwGetKey(_window, GLFW_KEY_2) == GLFW_PRESS) {
        if (!_keys.at(GLFW_KEY_2)) {
            _scene->previousCamera();
            _keys.at(GLFW_KEY_2) = true;
        }
    } else {
        _keys.at(GLFW_KEY_2) = false;
    }

    // Movement keys
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(DOWNWARD, deltaTime);

    // Mouse movement (handled in mouseCallback)
    if (fabsf(_mouseDeltaX) > EPSILON || fabsf(_mouseDeltaY) > EPSILON) {
        camera->processMouseMovement(_mouseDeltaX, _mouseDeltaY);
        _mouseDeltaX = 0.0f;
        _mouseDeltaY = 0.0f;
    }

    // Scroll input (handled in scrollCallback)
    if (fabsf(_scrollOffset) > EPSILON) {
        camera->processMouseScroll(_scrollOffset);
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
