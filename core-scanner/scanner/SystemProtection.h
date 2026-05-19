#pragma once
#include <vector>
#include "shared/types/AppConfig.h"
#include "../detection/IDetectionEngine.h"

class SystemProtection {
public:
    explicit SystemProtection(const AppConfig& config);
    std::vector<DetectionResult> verifySystem();
private:
    AppConfig config_;
};
