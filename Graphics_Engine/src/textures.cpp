#define STB_IMAGE_IMPLEMENTATION

#include "internal/textures.h"
#include <iostream>
#include <ostream>
#include <string>
#include <glad/glad.h>
#include "stb_image.h"

namespace Texture {
    // uses STB_image to create a texture from numerous types of image files
    unsigned int makeTexture(const std::string& path) {
        unsigned int textureID;
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load image: " << path << std::endl;
            throw std::runtime_error("Failed to load image");
        }

        int internalFormat = 0;
        int format = 0;

        // allows alpha channel to work on image formats that provide it
        if (nrChannels == 3) {
            //std::cout << "3 channels" << std::endl;
            internalFormat = GL_RGB;
            format = GL_RGB;
        } else if (nrChannels == 4) {
            //std::cout << "4 channels" << std::endl;
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
        } else {
            std::cout << "Unsupported format: " << path << std::endl;
            return 0;
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        return textureID;
    }
}