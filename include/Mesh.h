#pragma once

#include "struct.h"

class Mesh {
  public:
    Mesh(const std::shared_ptr<std::vector<Vertex>> &vertices,
         const std::vector<unsigned int>            &indices);

    ~Mesh();

    // Delete copy constructor and copy assignment operator
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    // Move constructor and move assignment operator
    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;

    void draw() const;

    void                             setModelMatrix(const glm::mat4 &modelMatrix);
    const glm::mat4                 &getModelMatrix() const;
    void                             setMaterial(const Material &material);
    const std::vector<Vertex>       &getVertices() const;
    const std::vector<unsigned int> &getIndices() const;
    const Material                  &getMaterial() const;

  private:
    std::shared_ptr<std::vector<Vertex>> _vertices;
    std::vector<unsigned int>            _indices;
    Material                             _material;
    unsigned int                         _VAO, _VBO, _EBO;
    glm::mat4                            _modelMatrix;

    void _setupMesh();
};
