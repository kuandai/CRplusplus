#include <texture.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Image loading tool by Sean Barrett. TODO: add proper license
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

namespace Marlin {
    // Texture Atlas
    std::map<std::string, Texture> textureAtlas;

    void loadNewTexture(std::string path, std::string name) {
        // Temporary Texture struct
        Texture newTexture;

        // stbi loads the image
        unsigned char *data = stbi_load(path.c_str(), &newTexture.width, &newTexture.height, &newTexture.nrChannels, 0);

        // Create texture
        glGenTextures(1, &newTexture.texture);
        glBindTexture(GL_TEXTURE_2D, newTexture.texture);

        // Configure texture
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newTexture.width, newTexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load image: " << path << std::endl;
        }

        // Free the image memory
        stbi_image_free(data);

        // Add the texture into atlas
        Marlin::textureAtlas[name] = newTexture;
    }
}