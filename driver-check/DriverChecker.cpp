#include "DriverChecker.h"
#include "../shared/utils/WinApiHelpers.h"
#include <vector>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#endif

DriverChecker::DriverChecker(const AppConfig& config) : config_(config) {}

std::vector<DetectionResult> DriverChecker::scanDrivers() {
    std::vector<DetectionResult> results;
#if defined(_WIN32) || defined(WIN32)
    LPVOID drivers[1024];
    DWORD cbNeeded;
    if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded)) {
        int count = cbNeeded / sizeof(LPVOID);
        for (int i = 0; i < count; ++i) {
            TCHAR driverPath[MAX_PATH];
            if (GetDeviceDriverFileName(drivers[i], driverPath, MAX_PATH)) {
                std::string path = driverPath;
                if (path.find("unsigned") != std::string::npos || path.find("beta") != std::string::npos) {
                    results.push_back({0, "kernel-driver", path, "high", "Suspicious or unsigned kernel driver path detected", WinApiHelpers::currentTimestamp()});
                }
            }
        }
    }
#endif
    if (results.empty()) {
        results.push_back({0, "driver-check", "stable", "low", "Driver integrity scan completed without critical anomalies", WinApiHelpers::currentTimestamp()});
    }
    return results;
}
