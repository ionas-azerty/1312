#pragma once
#include <string>
#include <vector>

struct AppConfig {
    std::string cloudEndpoint = "http://127.0.0.1:4000";
    int sensitivity = 70;
    bool enableAgentHooks = true;
    std::vector<std::string> whitelist;
    void loadDefaults() {
        sensitivity = 70;
        cloudEndpoint = "http://127.0.0.1:4000";
        whitelist = {"explorer.exe", "vortex-backend.exe", "node.exe"};
    }
};
