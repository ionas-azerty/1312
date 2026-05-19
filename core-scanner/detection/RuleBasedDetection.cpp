#include "RuleBasedDetection.h"
#include "../../../shared/utils/StringUtils.h"

std::vector<DetectionResult> RuleBasedDetection::detect(const ProcessInfo& process) {
    std::vector<DetectionResult> results;
    const std::vector<std::pair<std::string, std::string>> rules = {
        {"cheatengine", "Known cheat tool detected"},
        {"x64dbg", "Known debugger detected"},
        {"processhacker", "Process management tool can be used for injection"},
        {"ollydbg", "Debugger detected"},
        {"idag", "Reverse-engineering tool detected"},
        {"megaheg", "Hidden injection tool detected"}
    };
    for (auto& rule : rules) {
        if (StringUtils::containsInsensitive(process.processName, rule.first) ||
            StringUtils::containsInsensitive(process.executablePath, rule.first) ||
            StringUtils::containsInsensitive(process.windowClass, rule.first)) {
            results.push_back({process.processId, process.processName, process.executablePath, "high", rule.second, ""});
        }
    }
    return results;
}
