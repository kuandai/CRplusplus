#include <render/window.hpp>
#include <input.hpp>
#include <globals.hpp>
#include <config.hpp>

#include <iostream>
#include <thread>
#include <atomic>
#include <fstream>
#include <sstream>

#include <ryml.hpp>
#include <ryml_std.hpp> // std::string compatibility

#include <spdlog/spdlog.h>

// Shared flag to signal when to shut down game
std::atomic<bool> gameRunning(true);  // Shared flag to signal when to stop

int main()
{
    // Set log layout
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%l%$] %v");

    // TRACE 0
    // DEBUG 1
    // INFO 2
    // WARN 3
    // ERROR 4
    // CRITICAL 5
    // OFF 6
    int logLevel = 0;

    // Load settings file
    if (Marlin::config::init(std::ifstream("./res/settings.yaml"))) {
        spdlog::warn("Settings parsing failed. Using defaults");
    } else {
        Marlin::config::cfgRoot["logLevel"] >> logLevel;
    }

    // Apply applicable settings
    spdlog::set_level(static_cast<spdlog::level::level_enum>(logLevel));
    spdlog::info(std::format("Log level: {}", logLevel));

    spdlog::info("Cosmic Reach++");

    // Create Game Window
    Marlin::GameWindow game;

    while (gameRunning) {}

    // Quit
    spdlog::info("Exiting game. Have a good day");
    return 0;
}
