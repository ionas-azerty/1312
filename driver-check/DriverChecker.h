#pragma once
#include <vector>
#include "../shared/types/AppConfig.h"
#include "../core-scanner/detection/IDetectionEngine.h"

class DriverChecker {
public:
    explicit DriverChecker(const AppConfig& config);
    std::vector<DetectionResult> scanDrivers();
private:
    AppConfig config_;
};
