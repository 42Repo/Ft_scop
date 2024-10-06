
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

typedef unsigned int GLenum;
typedef int          GLint;

class Shader {
  public:
    Shader();
    ~Shader();

    // Delete copy constructor and assignment operator to prevent copying
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    // Move constructor and assignment operator
    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    // Add a shader stage from a file
    void addShaderFromFile(const std::string &filePath, GLenum shaderType);

    // Add a shader stage from a string
    void addShaderFromSource(const std::string &sourceCode, GLenum shaderType);

    // Link all added shaders into a shader program
    void link();

    // Use/activate the shader program
    void use() const;

    // Utility functions to set uniform variables in the shaders
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;

    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;

    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

  private:
    unsigned int              ID;        // Shader program ID
    std::vector<unsigned int> shaderIDs; // Store shader IDs for all shader stages

    // Utility function for checking shader compilation/linking errors
    void checkCompileErrors(unsigned int shader, const std::string &type) const;

    // Helper function to load shader source code from a file
    std::string loadShaderSource(const std::string &filePath) const;
};
