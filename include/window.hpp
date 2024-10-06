#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Marlin {
    // The Window
    extern GLFWwindow* window;
    
    // Callback function for resizing framebuffer upon window resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Render Thread
    void renderThread();
}