#include <render/texture.hpp>

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

    // Constructor for Mesh
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        generateMesh();
    }

    // Mesh initialization
    void Mesh::generateMesh() {
        // Initialize OpenGL buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Position Attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex Attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);

        // Texture Coordinate Attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
}