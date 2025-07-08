#include <config.hpp>

#include <spdlog/spdlog.h>

namespace Marlin::config {
    ryml::Tree cfgTree;
    ryml::NodeRef cfgRoot;
    bool parseFailed = false; // Flag to determine if a parse failed.

    int init(std::ifstream cfgFile) {
        // Set error callbacks
        ryml::Callbacks callbacks;
        callbacks.m_error = [](const char* msg, size_t len, c4::yml::Location loc, void* user_data) {
            spdlog::warn(std::format("YAML Parsing Error: {}", std::string(msg, len)));
            Marlin::config::parseFailed = true;
        };
        ryml::set_callbacks(callbacks);

        // Ensure file is open
        if (!cfgFile.is_open()) {
            spdlog::warn("Unable to open settings file.");
            return 1;
        } else {
            // Parse config
            std::string cfgDataString((std::istreambuf_iterator<char>(cfgFile)),
                                std::istreambuf_iterator<char>());
            Marlin::config::parseFailed = false;
            cfgTree = ryml::parse_in_place(ryml::to_substr(cfgDataString));
            cfgRoot = cfgTree.rootref();
            if (parseFailed) { return 1; } // Flag will be set if parsing fails
        }
        return 0;
    }
}