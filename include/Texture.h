#pragma once

#include "struct.h"

class Texture {
  public:
    Texture(const std::string &path, GLenum textureType = GL_TEXTURE_2D, bool flip = true);
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

    unsigned int getID() const { return ID; }

  private:
    unsigned int ID;
    GLenum       type;

    unsigned char *_loadImage(const std::string &path, int &width, int &height, int &nrChannels,
                              bool flip);
};
