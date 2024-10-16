#pragma once

#include "struct.h"

// Directions for keyboard movement
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UPWARD, DOWNWARD };

class Camera {
  public:
    Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 3.0f),
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
           float pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void      processKeyboard(Camera_Movement direction, float deltaTime);
    void      processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void      processMouseScroll(float yoffset);
    void      setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const;
    void      setFieldOfView(float fov);
    float     getFieldOfView() const;
    void      setAspectRatio(float aspectRatio);
    float     getAspectRatio() const;

  private:
    // Camera Attributes
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;

    // Euler Angles
    float _yaw;
    float _pitch;

    // Camera options
    float _movementSpeed;
    float _mouseSensitivity;
    float _fieldOfView;

    // Projection options
    float _aspectRatio;
    float _nearPlane;
    float _farPlane;

    void _updateCameraVectors();
};
