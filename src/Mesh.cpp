#include "../include/Mesh.h"
#include "../include/glad/glad.h"

Mesh::Mesh(const std::shared_ptr<std::vector<Vertex>> &vertices,
           const std::vector<unsigned int>            &indices)
    : _vertices(vertices),
      _indices(indices),
      _VAO(0),
      _VBO(0),
      _EBO(0),
      _modelMatrix(glm::mat4(1.0f)) {
    _setupMesh();
}

Mesh::~Mesh() {
    // Delete OpenGL resources
    if (_VAO != 0)
        glDeleteVertexArrays(1, &_VAO);
    if (_VBO != 0)
        glDeleteBuffers(1, &_VBO);
    if (_EBO != 0)
        glDeleteBuffers(1, &_EBO);
}

Mesh::Mesh(Mesh &&other) noexcept
    : _vertices(std::move(other._vertices)),
      _indices(std::move(other._indices)),
      _VAO(other._VAO),
      _VBO(other._VBO),
      _EBO(other._EBO) {
    other._VAO = 0;
    other._VBO = 0;
    other._EBO = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
    if (this != &other) {
        // Clean up existing resources
        if (_VAO != 0)
            glDeleteVertexArrays(1, &_VAO);
        if (_VBO != 0)
            glDeleteBuffers(1, &_VBO);
        if (_EBO != 0)
            glDeleteBuffers(1, &_EBO);

        // Transfer ownership
        _vertices = std::move(other._vertices);
        _indices = std::move(other._indices);
        _VAO = other._VAO;
        _VBO = other._VBO;
        _EBO = other._EBO;

        other._VAO = 0;
        other._VBO = 0;
        other._EBO = 0;
    }
    return *this;
}

void Mesh::_setupMesh() {
    // Generate buffers and arrays
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    // Bind _VAO
    glBindVertexArray(_VAO);

    // Load vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_vertices->size() * sizeof(Vertex)),
                 _vertices->data(), GL_STATIC_DRAW);
    // Load index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(_indices.size() * sizeof(unsigned int)), _indices.data(),
                 GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, position)));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, normal)));

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    // Unbind _VAO
    glBindVertexArray(0);
}

void Mesh::draw() const {
    // Bind _VAO and draw elements
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT,
                   reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
    glBindVertexArray(0);
}

void Mesh::setModelMatrix(const glm::mat4 &modelMatrix) { _modelMatrix = modelMatrix; }

const glm::mat4 &Mesh::getModelMatrix() const { return _modelMatrix; }

void Mesh::setMaterial(const Material &material) { _material = material; }

const std::vector<Vertex> &Mesh::getVertices() const { return *_vertices; }

const std::vector<unsigned int> &Mesh::getIndices() const { return _indices; }

const Material &Mesh::getMaterial() const { return _material; }
