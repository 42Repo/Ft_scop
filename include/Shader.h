
#pragma once

#include "struct.h"

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

    void addShaderFromFile(const std::string &filePath, GLenum shaderType);
    void addShaderFromSource(const std::string &sourceCode, GLenum shaderType);

    void link();

    void use() const;

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
    unsigned int              ID;
    std::vector<unsigned int> shaderIDs;

    void _checkCompileErrors(unsigned int shader, const std::string &type) const;

    std::string _loadShaderSource(const std::string &filePath) const;
};
