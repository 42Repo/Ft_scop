#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

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

  private:
    // Mesh Data
    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;

    // OpenGL buffers
    unsigned int _VAO, _VBO, _EBO;

    // Initializes the buffer objects and vertex attributes
    void setupMesh();
};
