#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Forward declarations
class Shader;
class Camera;
class Mesh;
class Texture;

class Scene {
  public:
    // Constructor and Destructor
    Scene();
    ~Scene();

    // Delete copy constructor and copy assignment operator
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    // Add a mesh to the scene
    void addMesh(const std::shared_ptr<Mesh> &mesh);

    // Add a texture to the scene
    void addTexture(const std::shared_ptr<Texture> &texture);

    // Add a shader to the scene
    void addShader(const std::shared_ptr<Shader> &shader);

    // Add a camera to the scene
    void addCamera(const std::shared_ptr<Camera> &camera);

    // Set the active camera
    void setActiveCamera(size_t index);

    // Get the active camera
    std::shared_ptr<Camera> getActiveCamera() const;

    // Update the scene (e.g., handle input, update animations)
    void update(float deltaTime);

    // Render the scene
    void render();

  private:
    // Containers for scene elements
    std::vector<std::shared_ptr<Mesh>>    meshes;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<std::shared_ptr<Shader>>  shaders;
    std::vector<std::shared_ptr<Camera>>  cameras;

    // Index of the active camera
    size_t activeCameraIndex;

    // Helper function to render all meshes
    void renderMeshes();
};
