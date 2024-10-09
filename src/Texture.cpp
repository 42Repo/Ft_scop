#include "../include/Texture.h"
#include "../include/stb_image.h"
#include <iostream>

Texture::Texture(const std::string &path, GLenum textureType, bool flip)
    : ID(0),
      type(textureType) {
    // Generate texture ID
    glGenTextures(1, &ID);

    // Load image data
    int            width, height, nrChannels;
    unsigned char *data = _loadImage(path, width, height, nrChannels, flip);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            throw std::runtime_error("Unsupported texture format");
        }

        // Bind texture
        glBindTexture(type, ID);

        // Upload image data to texture
        glTexImage2D(type, 0, static_cast<GLint>(format), width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);

        // Generate mipmaps
        glGenerateMipmap(type);

        // Default texture parameters (can be changed using setParameter)
        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Unbind texture
        glBindTexture(type, 0);

        // Free image data
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture");
    }
}

Texture::~Texture() {
    if (ID != 0) {
        glDeleteTextures(1, &ID);
    }
}

Texture::Texture(Texture &&other) noexcept : ID(other.ID), type(other.type) { other.ID = 0; }

Texture &Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
        ID = other.ID;
        type = other.type;
        other.ID = 0;
    }
    return *this;
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, ID);
}

void Texture::setParameter(GLenum name, GLint value) {
    glBindTexture(type, ID);
    glTexParameteri(type, name, value);
    glBindTexture(type, 0);
}

void Texture::setParameter(GLenum name, GLfloat *value) {
    glBindTexture(type, ID);
    glTexParameterfv(type, name, value);
    glBindTexture(type, 0);
}

unsigned char *Texture::_loadImage(const std::string &path, int &width, int &height,
                                   int &nrChannels, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    return data;
}
