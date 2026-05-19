#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../types/AppConfig.h"

namespace FileUtils {
    inline bool exists(const std::string& path) {
        std::ifstream file(path);
        return file.good();
    }

    inline bool readText(const std::string& path, std::string& output) {
        std::ifstream file(path);
        if (!file.is_open()) return false;
        std::ostringstream buffer;
        buffer << file.rdbuf();
        output = buffer.str();
        return true;
    }

    inline bool writeText(const std::string& path, const std::string& content) {
        std::ofstream file(path);
        if (!file.is_open()) return false;
        file << content;
        return true;
    }

    inline bool readJson(const std::string& path, AppConfig& config) {
        std::string payload;
        if (!readText(path, payload)) return false;
        auto getString = [&](const std::string& key, std::string& target) {
            auto pos = payload.find(key);
            if (pos == std::string::npos) return false;
            auto start = payload.find(':', pos);
            if (start == std::string::npos) return false;
            start = payload.find('"', start);
            if (start == std::string::npos) return false;
            auto end = payload.find('"', start + 1);
            if (end == std::string::npos) return false;
            target = payload.substr(start + 1, end - start - 1);
            return true;
        };
        getString("cloudEndpoint", config.cloudEndpoint);
        return true;
    }
}
