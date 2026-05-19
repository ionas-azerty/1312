#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "shared/types/AppConfig.h"
#include "shared/utils/FileUtils.h"
#include "shared/utils/WinApiHelpers.h"
#include "core-scanner/scanner/ProcessScanner.h"
#include "core-scanner/scanner/MemoryModuleMonitor.h"
#include "core-scanner/scanner/ScreenshotProtection.h"
#include "core-scanner/scanner/SystemProtection.h"
#include "driver-check/DriverChecker.h"

static std::string formatJsonString(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());
    for (char ch : value) {
        switch (ch) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += ch; break;
        }
    }
    return escaped;
}

static std::string buildJsonArray(const std::vector<std::string>& entries) {
    std::ostringstream out;
    out << "[";
    for (size_t i = 0; i < entries.size(); ++i) {
        out << entries[i];
        if (i + 1 < entries.size()) out << ",";
    }
    out << "]";
    return out.str();
}

static std::string detectionResultToJson(const DetectionResult& result) {
    std::ostringstream out;
    out << "{"
        << "\"processId\":" << result.processId << ","
        << "\"processName\":\"" << formatJsonString(result.processName) << "\",";
    out << "\"path\":\"" << formatJsonString(result.executablePath) << "\",";
    out << "\"riskLevel\":\"" << formatJsonString(result.riskLevel) << "\",";
    out << "\"reason\":\"" << formatJsonString(result.reason) << "\",";
    out << "\"timestamp\":\"" << formatJsonString(result.timestamp) << "\"";
    out << "}";
    return out.str();
}

int main(int argc, char* argv[]) {
    AppConfig config;
    std::string configPath = "config/appsettings.json";
    if (!FileUtils::readJson(configPath, config)) {
        config.loadDefaults();
    }

    if (argc <= 1) {
        std::cout << "{\"status\":\"ready\",\"message\":\"Vortex backend waiting for commands\"}";
        return 0;
    }

    std::string command = argv[1];
    if (command == "--scan") {
        ProcessScanner scanner(config);
        auto results = scanner.scanProcesses();
        std::vector<std::string> jsonEntries;
        for (auto& entry : results) {
            jsonEntries.push_back(detectionResultToJson(entry));
        }
        std::cout << "{\"status\":\"ok\",\"results\":" << buildJsonArray(jsonEntries) << "}";
        return 0;
    }

    if (command == "--memory") {
        MemoryModuleMonitor monitor(config);
        auto entries = monitor.inspectMemory();
        std::vector<std::string> jsonEntries;
        for (auto& entry : entries) {
            jsonEntries.push_back(detectionResultToJson(entry));
        }
        std::cout << "{\"status\":\"ok\",\"memory\":" << buildJsonArray(jsonEntries) << "}";
        return 0;
    }

    if (command == "--driver-check") {
        DriverChecker checker(config);
        auto entries = checker.scanDrivers();
        std::vector<std::string> jsonEntries;
        for (auto& entry : entries) {
            jsonEntries.push_back(detectionResultToJson(entry));
        }
        std::cout << "{\"status\":\"ok\",\"drivers\":" << buildJsonArray(jsonEntries) << "}";
        return 0;
    }

    if (command == "--screenshot") {
        ScreenshotProtection protection(config);
        auto entries = protection.captureAndAnalyze();
        std::vector<std::string> jsonEntries;
        for (auto& entry : entries) {
            jsonEntries.push_back(detectionResultToJson(entry));
        }
        std::cout << "{\"status\":\"ok\",\"screenshots\":" << buildJsonArray(jsonEntries) << "}";
        return 0;
    }

    if (command == "--status") {
        SystemProtection protection(config);
        auto checks = protection.verifySystem();
        std::vector<std::string> jsonEntries;
        for (auto& entry : checks) {
            jsonEntries.push_back(detectionResultToJson(entry));
        }
        std::cout << "{\"status\":\"ok\",\"system\":" << buildJsonArray(jsonEntries) << "}";
        return 0;
    }

    std::cout << "{\"status\":\"error\",\"message\":\"Unknown command\"}";
    return 1;
}
