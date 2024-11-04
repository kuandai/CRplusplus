#pragma once

#include <vector>
#include <GLFW/glfw3.h>

namespace Marlin {
    extern std::vector<int> keyboard;

    /*
    * Callback function to continuously update key state
    */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}