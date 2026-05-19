#pragma once
#include <vector>
#include "shared/types/AppConfig.h"
#include "../detection/IDetectionEngine.h"

class ScreenshotProtection {
public:
    explicit ScreenshotProtection(const AppConfig& config);
    std::vector<DetectionResult> captureAndAnalyze();
private:
    AppConfig config_;
    std::vector<std::string> detectOverlayApplications();
};
