#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

namespace Marlin {
    // The Window
    extern GLFWwindow* window;
    
    // Mouse is captured state
    extern bool isMouseCaptured;

    // Callback function for resizing framebuffer upon window resize
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Render Thread
    void renderThread();
}