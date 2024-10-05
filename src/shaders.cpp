#include "../include/shaders.h"
#include "../include/includes.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode, fragmentCode;
    try {
        std::ifstream vShaderFile(vertexPath);
        std::ifstream fShaderFile(fragmentPath);

		if (!vShaderFile.is_open() || !fShaderFile.is_open())
			throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (const std::ifstream::failure &e) {
		(void)e;
        throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() const { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location != -1)
        glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string &name, int value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location != -1)
        glUniform1i(location, value);
}

void Shader::setFloat(const std::string &name, float value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location != -1)
        glUniform1f(location, value);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string &type) const {
    int  success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" +
                                     infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" +
                                     infoLog);
        }
    }
}
