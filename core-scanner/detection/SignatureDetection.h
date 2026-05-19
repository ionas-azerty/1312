#pragma once
#include <vector>
#include "IDetectionEngine.h"

class SignatureDetection : public IDetectionEngine {
public:
    std::vector<DetectionResult> detect(const ProcessInfo& process) override;
};
