#pragma once

#include <ryml.hpp>
#include <ryml_std.hpp> // std::string compatibility

#include <fstream>

namespace Marlin::config {
    extern ryml::Tree cfgTree;
    extern ryml::ConstNodeRef cfgRoot;
    extern bool parseFailed;
    int init(std::ifstream cfgFile);
}