#include <render/window.hpp>
#include <input.hpp>
#include <globals.hpp>

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

    // Load settings file
    std::ifstream cfgFile("./res/settings.yaml");
    if (!cfgFile.is_open()) {
        spdlog::warn("Unable to open settings file. Using defaults.");
    } else {
        std::string cfgDataString((std::istreambuf_iterator<char>(cfgFile)),
                            std::istreambuf_iterator<char>());
        ryml::Tree cfgTree = ryml::parse_in_arena(ryml::to_csubstr(cfgDataString));
        ryml::NodeRef cfgRoot = cfgTree["root"];
        try {
            std::cout << cfgTree["logLevel"] << std::endl;
        } catch (const std::exception &e) {
        spdlog::error("YAML processing error: {}", e.what());
        }

    }

    spdlog::info("Cosmic Reach++");

    // Create Game Window
    Marlin::GameWindow game;

    while (gameRunning) {}

    // Quit
    spdlog::info("Exiting game. Have a good day");
    return 0;
}
