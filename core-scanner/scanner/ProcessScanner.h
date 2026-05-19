#pragma once
#include <vector>
#include <memory>
#include "../../../shared/types/AppConfig.h"
#include "../../../shared/types/ProcessInfo.h"
#include "../detection/IDetectionEngine.h"

class ProcessScanner {
public:
    explicit ProcessScanner(const AppConfig& config);
    std::vector<DetectionResult> scanProcesses();
private:
    AppConfig config_;
    std::vector<std::unique_ptr<IDetectionEngine>> engines_;
    void populateEngines();
    std::vector<ProcessInfo> enumerateProcesses();
    std::vector<std::string> collectModules(int pid);
    std::string detectWindowClass(int pid);
};
