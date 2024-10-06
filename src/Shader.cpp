#include "../include/Shader.h"
#include "../include/glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

Shader::Shader() : ID(0) {
    // Create a program object
    ID = glCreateProgram();
    if (ID == 0) {
        throw std::runtime_error("Failed to create shader program");
    }
}

Shader::~Shader() {
    // Delete the shader program
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

Shader::Shader(Shader &&other) noexcept : ID(other.ID), shaderIDs(std::move(other.shaderIDs)) {
    other.ID = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        // Delete existing resources
        if (ID != 0) {
            glDeleteProgram(ID);
        }
        // Transfer ownership
        ID = other.ID;
        shaderIDs = std::move(other.shaderIDs);
        other.ID = 0;
    }
    return *this;
}

// Helper function to convert shader type enum to string
static std::string shaderTypeToString(GLenum shaderType) {
    switch (shaderType) {
    case GL_VERTEX_SHADER:
        return "VERTEX";
    case GL_FRAGMENT_SHADER:
        return "FRAGMENT";
    case GL_GEOMETRY_SHADER:
        return "GEOMETRY";
    case GL_TESS_CONTROL_SHADER:
        return "TESS_CONTROL";
    case GL_TESS_EVALUATION_SHADER:
        return "TESS_EVALUATION";
    case GL_COMPUTE_SHADER:
        return "COMPUTE";
    default:
        return "UNKNOWN";
    }
}

void Shader::addShaderFromFile(const std::string &filePath, GLenum shaderType) {
    std::string shaderCode = loadShaderSource(filePath);
    addShaderFromSource(shaderCode, shaderType);
}

void Shader::addShaderFromSource(const std::string &sourceCode, GLenum shaderType) {
    const char *code = sourceCode.c_str();

    // Create shader object
    unsigned int shader = glCreateShader(shaderType);
    if (shader == 0) {
        throw std::runtime_error("Failed to create shader");
    }

    // Compile shader
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderTypeToString(shaderType));

    // Attach shader to the program
    glAttachShader(ID, shader);

    // Store the shader ID for later deletion
    shaderIDs.push_back(shader);
}

void Shader::link() {
    // Link the shader program
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete the shader objects after linking
    for (unsigned int shader : shaderIDs) {
        glDeleteShader(shader);
    }
    shaderIDs.clear();
}

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string &type) const {
    int  success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
            throw std::runtime_error("Shader compilation error");
        }
    } else {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
            throw std::runtime_error("Shader program linking error");
        }
    }
}

std::string Shader::loadShaderSource(const std::string &filePath) const {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream shaderStream;

    try {
        shaderFile.open(filePath);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
    } catch (std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ: " << filePath << "\n" << e.what() << std::endl;
        throw std::runtime_error("Failed to read shader file");
    }

    return shaderStream.str();
}
