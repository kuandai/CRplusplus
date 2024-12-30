#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <window.hpp>
#include <input.hpp>
#include <globals.hpp>

#include <iostream>
#include <thread>
#include <atomic>

#include <spdlog/spdlog.h>

// Shared flag to signal when to shut down game
std::atomic<bool> gameRunning(true);  // Shared flag to signal when to stop

int main()
{
    spdlog::info("Cosmic Reach++");

    // Create Game Window
    Marlin::GameWindow game;

    while (true) {}

    // Quit
    return 0;
}
