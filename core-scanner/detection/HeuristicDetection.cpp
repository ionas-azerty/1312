#include "HeuristicDetection.h"
#include "../../../shared/utils/StringUtils.h"

std::vector<DetectionResult> HeuristicDetection::detect(const ProcessInfo& process) {
    std::vector<DetectionResult> results;
    if (process.processName.length() > 0 && process.processName.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789_.\-") == std::string::npos) {
        if (process.processName.size() > 20 || process.processName.find_first_of("0123456789") != std::string::npos) {
            results.push_back({process.processId, process.processName, process.executablePath, "medium", "Randomized process name pattern detected", ""});
        }
    }
    if (StringUtils::containsInsensitive(process.executablePath, "temp") || StringUtils::containsInsensitive(process.executablePath, "appdata")) {
        results.push_back({process.processId, process.processName, process.executablePath, "medium", "Executable is running from a suspicious path", ""});
    }
    return results;
}
