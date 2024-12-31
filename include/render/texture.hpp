#pragma once

#include <glm/glm.hpp>

#include <render/shader.hpp>

#include <vector>
#include <string>
#include <map>

namespace Marlin {
    // Struct for textures
    struct Texture {
        unsigned int texture;
        int width, height, nrChannels;
        std::string type;
    };

    // Struct for verticies
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    // Mesh class
    class Mesh {
        public:
            // Vertex & Texture data
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            // Constructor
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
            void Draw(Shader &shader);

        private:
            // OpenGL data
            unsigned int VAO, VBO, EBO;

            void generateMesh();
    };

    // Texture Atlas
    extern std::map<std::string, Texture> textureAtlas;

    void loadNewTexture(std::string path, std::string name);
}