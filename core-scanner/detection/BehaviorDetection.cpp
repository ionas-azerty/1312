#include "BehaviorDetection.h"
#include "../../../shared/utils/StringUtils.h"

std::vector<DetectionResult> BehaviorDetection::detect(const ProcessInfo& process) {
    std::vector<DetectionResult> results;
    for (auto& module : process.loadedModules) {
        if (StringUtils::containsInsensitive(module, "inject") || StringUtils::containsInsensitive(module, "hook")) {
            results.push_back({process.processId, process.processName, process.executablePath, "high", "Suspicious module or hook detected", ""});
            break;
        }
    }
    if (StringUtils::containsInsensitive(process.windowClass, "overlay") || StringUtils::containsInsensitive(process.windowClass, "gameoverlay")) {
        results.push_back({process.processId, process.processName, process.executablePath, "medium", "Overlay window class is suspicious", ""});
    }
    return results;
}
