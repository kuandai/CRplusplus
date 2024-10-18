#pragma once

#include <string>
#include <map>

namespace Marlin {
    // Struct to hold gl texture
    struct Texture {
        unsigned int texture;
        int width, height, nrChannels;
    };

    // Texture Atlas
    extern std::map<std::string, Texture> textureAtlas;

    void loadNewTexture(std::string path, std::string name);
}