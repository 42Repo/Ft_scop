#include "../include/Scene.h"
#include "../include/Camera.h"
#include "../include/Mesh.h"
#include "../include/Shader.h"
#include "../include/Texture.h"
#include "../include/glad/glad.h"

Scene::Scene() : activeCameraIndex(0) {
    // Constructor implementation (if needed)
}

Scene::~Scene() {
    // Destructor implementation (if needed)
}

void Scene::addMesh(const std::shared_ptr<Mesh> &mesh) { meshes.push_back(mesh); }

void Scene::addTexture(const std::shared_ptr<Texture> &texture) { textures.push_back(texture); }

void Scene::addShader(const std::shared_ptr<Shader> &shader) { shaders.push_back(shader); }

void Scene::addCamera(const std::shared_ptr<Camera> &camera) { cameras.push_back(camera); }

void Scene::setActiveCamera(size_t index) {
    if (index < cameras.size()) {
        activeCameraIndex = index;
    }
}

std::shared_ptr<Camera> Scene::getActiveCamera() const {
    if (cameras.empty()) {
        return nullptr;
    }
    return cameras[activeCameraIndex];
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
    renderMeshes();
}

void Scene::renderMeshes() {
    for (const auto &mesh : meshes) {
        if (shaders.empty()) {
            continue;
        }

        auto shader = shaders[0];
        shader->use();

        // Set camera uniforms
        shader->setMat4("view", getActiveCamera()->getViewMatrix());
        shader->setMat4("projection", getActiveCamera()->getProjectionMatrix());

        // **Set the mesh's model matrix**
        shader->setMat4("model", mesh->getModelMatrix());

        // Bind textures
        for (size_t i = 0; i < textures.size(); ++i) {
            textures[i]->bind(static_cast<unsigned int>(i));
            shader->setInt("texture" + std::to_string(i + 1), static_cast<int>(i));
        }

        // Draw the mesh
        mesh->draw();
    }
}
