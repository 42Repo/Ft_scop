#pragma once

#include <vector>
#include "Mesh.h"
#include "Shader.h"

class Scene {
public:
    Scene();
    ~Scene();

    // Add an object to the scene
    void addObject(const Mesh& mesh);

    // Render all objects in the scene
    void render(const Shader& shader, const Camera& camera);

    // Update scene (e.g., animations, physics)
    void update(float deltaTime);

private:
    std::vector<Mesh> objects; // List of objects in the scene
};
