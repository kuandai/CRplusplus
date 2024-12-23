#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window.hpp>
#include <input.hpp>
#include <globals.hpp>

#include <iostream>
#include <thread>
#include <atomic>

#include "spdlog/spdlog.h"

// Shared flag to signal when to shut down game
std::atomic<bool> gameRunning(true);  // Shared flag to signal when to stop

int main()
{
    spdlog::info("Cosmic Reach++");

    glfwInit();

    // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile: reduced feature set, no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    Marlin::window = glfwCreateWindow(800, 600, "Cosmic Reach++", NULL, NULL);
    if (Marlin::window == NULL)
    {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Marlin::window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("Failed to initialize GLAD");
        return -1;
    }

    // Set Viewport - should be same as window size
    glViewport(0, 0, 800, 600);

    // Register framebuffer resize callback
    glfwSetFramebufferSizeCallback(Marlin::window, Marlin::framebuffer_size_callback);

    // Register keyboard update callback
    glfwSetKeyCallback(Marlin::window, Marlin::keyCallback);

    // Register mouse update callback
    glfwSetCursorPosCallback(Marlin::window, Marlin::mouseCallback);

    // Set cursor mode
    glfwSetInputMode(Marlin::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Create Render thread
    // TODO: get GLAD to cooperate and not segfault, main thread is render
    // thread for time being. Put all other thread creations above this line.
    Marlin::renderThread();

    // Wait for threads to finish

    // Quit
    return 0;
}
