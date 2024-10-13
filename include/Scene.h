#pragma once

#include "struct.h"

// Forward declarations
class Shader;
class Camera;
class Mesh;
class Texture;

class Scene {
  public:
    Scene();
    ~Scene();

    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

    void addMesh(const std::shared_ptr<Mesh> &mesh);
    void addTexture(const std::shared_ptr<Texture> &texture);
    void addShader(const std::shared_ptr<Shader> &shader);
    void addCamera(const std::shared_ptr<Camera> &camera);

    void setActiveCamera(size_t index);
    void nextCamera();
    void previousCamera();

    std::shared_ptr<Camera>               getActiveCamera() const;
    std::vector<std::shared_ptr<Camera>> &getCameras();

    void update(float deltaTime);
    void render();

  private:
    std::vector<std::shared_ptr<Mesh>>    _meshes;
    std::vector<std::shared_ptr<Texture>> _textures;
    std::vector<std::shared_ptr<Shader>>  _shaders;
    std::vector<std::shared_ptr<Camera>>  _cameras;

    size_t _activeCameraIndex;

    void _renderMeshes();
};
