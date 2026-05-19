#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../types/AppConfig.h"

namespace FileUtils {
    bool exists(const std::string& path);
    bool readText(const std::string& path, std::string& output);
    bool writeText(const std::string& path, const std::string& content);
    bool readJson(const std::string& path, AppConfig& config);
}
