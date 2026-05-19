#include "SystemProtection.h"
#include "shared/utils/WinApiHelpers.h"

SystemProtection::SystemProtection(const AppConfig& config) : config_(config) {}

std::vector<DetectionResult> SystemProtection::verifySystem() {
    std::vector<DetectionResult> results;
    if (WinApiHelpers::isDebuggerActive()) {
        results.push_back({0, "system-check", "debugger", "high", "Active debugger detected during anti-tamper verification", WinApiHelpers::currentTimestamp()});
    }
    if (WinApiHelpers::hasVirtualMachineArtifacts()) {
        results.push_back({0, "system-check", "virtualization", "medium", "Virtual machine artifacts detected", WinApiHelpers::currentTimestamp()});
    }
    if (results.empty()) {
        results.push_back({0, "system-check", "health", "low", "Integrity and anti-tamper checks passed", WinApiHelpers::currentTimestamp()});
    }
    return results;
}
