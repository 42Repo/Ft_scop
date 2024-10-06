#pragma once

#include "glad/glad.h"
#include <string>

class Texture {
  public:
    // Constructor: Loads texture from file
    Texture(const std::string &path, GLenum textureType = GL_TEXTURE_2D, bool flip = true);

    // Destructor
    ~Texture();

    // Delete copy constructor and copy assignment operator
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    // Move constructor and move assignment operator
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;

    // Bind the texture to a texture unit
    void bind(unsigned int unit = 0) const;

    // Set texture parameters
    void setParameter(GLenum name, GLint value);
    void setParameter(GLenum name, GLfloat *value);

    // Get the texture ID
    unsigned int getID() const { return ID; }

  private:
    unsigned int ID;
    GLenum       type;

    // Helper function to load image data
    unsigned char *loadImage(const std::string &path, int &width, int &height, int &nrChannels,
                             bool flip);
};
