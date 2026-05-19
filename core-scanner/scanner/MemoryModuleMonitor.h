#pragma once
#include <vector>
#include "shared/types/AppConfig.h"
#include "../detection/IDetectionEngine.h"

class MemoryModuleMonitor {
public:
    explicit MemoryModuleMonitor(const AppConfig& config);
    std::vector<DetectionResult> inspectMemory();
private:
    AppConfig config_;
    std::vector<std::string> scanSuspiciousModules();
};
