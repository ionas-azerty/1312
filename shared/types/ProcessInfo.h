#pragma once
#include <string>

struct ProcessInfo {
    int processId;
    std::string processName;
    std::string executablePath;
    std::string windowClass;
    std::vector<std::string> loadedModules;
};
