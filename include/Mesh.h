#pragma once

#include "struct.h"

class Mesh {
  public:
    // Constructor
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    // Destructor
    ~Mesh();

    // Delete copy constructor and copy assignment operator
    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    // Move constructor and move assignment operator
    Mesh(Mesh &&other) noexcept;
    Mesh &operator=(Mesh &&other) noexcept;

    // Render the mesh
    void draw() const;

    // **Model matrix methods**
    void             setModelMatrix(const glm::mat4 &modelMatrix);
    const glm::mat4 &getModelMatrix() const;
    void             setMaterial(const Material &material);
    const Material  &getMaterial() const;

  private:
    // Mesh Data
    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;
    Material                  _material;

    // OpenGL buffers
    unsigned int _VAO, _VBO, _EBO;

    // **Model matrix**
    glm::mat4 _modelMatrix;

    // Initializes the buffer objects and vertex attributes
    void _setupMesh();
};
