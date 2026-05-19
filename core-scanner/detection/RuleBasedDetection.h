#pragma once
#include <vector>
#include <string>
#include "IDetectionEngine.h"

class RuleBasedDetection : public IDetectionEngine {
public:
    std::vector<DetectionResult> detect(const ProcessInfo& process) override;
};
