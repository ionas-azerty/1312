#pragma once
#include <string>
#include <vector>
#include "../../../shared/types/ProcessInfo.h"

struct DetectionResult {
    int processId;
    std::string processName;
    std::string executablePath;
    std::string riskLevel;
    std::string reason;
    std::string timestamp;
};

class IDetectionEngine {
public:
    virtual ~IDetectionEngine() = default;
    virtual std::vector<DetectionResult> detect(const ProcessInfo& process) = 0;
};
