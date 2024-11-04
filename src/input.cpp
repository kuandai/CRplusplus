#include <input.hpp>
#include <iostream>

namespace Marlin{
    std::vector<int> keyboard(GLFW_KEY_LAST+1, GLFW_RELEASE);

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key >= 0 && key < keyboard.size()) {
            keyboard[key] = action;  // Store the action (GLFW_PRESS, GLFW_RELEASE)
        }
    }
}