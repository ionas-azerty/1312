#pragma once
#include <string>

struct LogEntry {
    std::string timestamp;
    std::string category;
    std::string message;
    std::string severity;
};
