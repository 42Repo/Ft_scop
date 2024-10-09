#include "../include/Scene.h"
#include "../include/Camera.h"
#include "../include/Mesh.h"
#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/glad/glad.h"

Scene::Scene() : _activeCameraIndex(0) {
    // Constructor implementation (if needed)
}

Scene::~Scene() {
    // Destructor implementation (if needed)
}

void Scene::addMesh(const std::shared_ptr<Mesh> &mesh) { _meshes.push_back(mesh); }

void Scene::addTexture(const std::shared_ptr<Texture> &texture) { _textures.push_back(texture); }

void Scene::addShader(const std::shared_ptr<Shader> &shader) { _shaders.push_back(shader); }

void Scene::addCamera(const std::shared_ptr<Camera> &camera) { _cameras.push_back(camera); }

void Scene::setActiveCamera(size_t index) {
    if (index < _cameras.size()) {
        _activeCameraIndex = index;
    }
}

void Scene::nextCamera() {
    if (!_cameras.empty()) {
        _activeCameraIndex = (_activeCameraIndex + 1) % _cameras.size();
    }
}

void Scene::previousCamera() {
    if (!_cameras.empty()) {
        _activeCameraIndex = (_activeCameraIndex + _cameras.size() - 1) % _cameras.size();
    }
}

std::shared_ptr<Camera> Scene::getActiveCamera() const {
    if (_cameras.empty()) {
        return nullptr;
    }
    return _cameras[_activeCameraIndex];
}

std::vector<std::shared_ptr<Camera>>& Scene::getCameras()
{
    return _cameras;
}

void Scene::render() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Get the active camera's view and projection matrices
    auto camera = getActiveCamera();
    if (!camera) {
        return;
    }

    // Render all meshes with their associated shaders and textures
    _renderMeshes();
}

void Scene::_renderMeshes() {
    for (const auto &mesh : _meshes) {
        if (_shaders.empty()) {
            continue;
        }

        auto shader = _shaders[0];
        shader->use();

        // Set camera uniforms
        shader->setMat4("view", getActiveCamera()->getViewMatrix());
        shader->setMat4("projection", getActiveCamera()->getProjectionMatrix());

        // **Set the mesh's model matrix**
        shader->setMat4("model", mesh->getModelMatrix());

        // Bind textures
        for (size_t i = 0; i < _textures.size(); ++i) {
            _textures[i]->bind(static_cast<unsigned int>(i));
            shader->setInt("texture" + std::to_string(i + 1), static_cast<int>(i));
        }

        // Draw the mesh
        mesh->draw();
    }
}
