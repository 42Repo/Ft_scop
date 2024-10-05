#pragma once

#include <iostream>

class Shader {
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

	Shader(const Shader &src) = delete;
	Shader &operator=(const Shader &rhs) = delete;
	
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

  private:
    unsigned int ID;

    void checkCompileErrors(unsigned int shader, const std::string &type) const;
};
