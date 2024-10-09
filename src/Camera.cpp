#include "../include/Camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, float yaw, float pitch)
    : _position(position),
      _worldUp(up),
      _yaw(yaw),
      _pitch(pitch),
      _movementSpeed(2.5f),
      _mouseSensitivity(0.1f),
      _fieldOfView(45.0f),
      _aspectRatio(800.0f / 600.0f),
      _nearPlane(0.1f),
      _farPlane(100.0f) {
    _updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const { return glm::lookAt(_position, _position + _front, _up); }

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(_fieldOfView), _aspectRatio, _nearPlane, _farPlane);
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = _movementSpeed * deltaTime;
    if (direction == FORWARD)
        _position += _front * velocity;
    if (direction == BACKWARD)
        _position -= _front * velocity;
    if (direction == LEFT)
        _position -= _right * velocity;
    if (direction == RIGHT)
        _position += _right * velocity;
    if (direction == UPWARD)
        _position += _worldUp * velocity;
    if (direction == DOWNWARD)
        _position -= _worldUp * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    _yaw += xoffset;
    _pitch += yoffset;

    // Constrain the pitch angle to prevent flipping
    if (constrainPitch) {
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
    }

    // Update front, right, and up vectors
    _updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    _fieldOfView -= yoffset;
    if (_fieldOfView < 1.0f)
        _fieldOfView = 1.0f;
    if (_fieldOfView > 45.0f)
        _fieldOfView = 45.0f;
}

void Camera::setPosition(const glm::vec3 &position) { _position = position; }

glm::vec3 Camera::getPosition() const { return _position; }

void Camera::setFieldOfView(float fov) { _fieldOfView = fov; }

float Camera::getFieldOfView() const { return _fieldOfView; }

void Camera::setAspectRatio(float aspectRatio) { _aspectRatio = aspectRatio; }

float Camera::getAspectRatio() const { return _aspectRatio; }

void Camera::_updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cosf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
    newFront.y = sinf(glm::radians(_pitch));
    newFront.z = sinf(glm::radians(_yaw)) * cosf(glm::radians(_pitch));
    _front = glm::normalize(newFront);

    // Also re-calculate the right and up vector
    _right = glm::normalize(glm::cross(_front, _worldUp)); // Normalize the vectors
    _up = glm::normalize(glm::cross(_right, _front));
}
