#pragma once

#include <vector>
#include <GLFW/glfw3.h>

namespace Marlin {
    extern std::vector<int> keyboard;

    /*
    * Callback function to continuously update key state
    */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /*
    * Callback function for handling mouse motion
    */
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    
    /*
    * Process key inputs for movement
    */
    void processInput(GLFWwindow* window, float deltaTime);
}