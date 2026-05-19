#include "ScreenshotProtection.h"
#include "shared/utils/WinApiHelpers.h"
#include <vector>
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

ScreenshotProtection::ScreenshotProtection(const AppConfig& config) : config_(config) {}

std::vector<std::string> ScreenshotProtection::detectOverlayApplications() {
    std::vector<std::string> overlays;
#if defined(_WIN32) || defined(WIN32)
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        char className[256] = {0};
        GetClassNameA(hwnd, className, sizeof(className));
        char title[256] = {0};
        GetWindowTextA(hwnd, title, sizeof(title));
        std::string full = std::string(className) + " " + std::string(title);
        std::vector<std::string> suspects = {"obs", "discord", "overlay", "steam", "geforce"};
        for (auto& candidate : suspects) {
            if (strstr(full.c_str(), candidate.c_str()) != nullptr) {
                auto* found = reinterpret_cast<std::vector<std::string>*>(lParam);
                found->push_back(full);
                break;
            }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&overlays));
#endif
    return overlays;
}

std::vector<DetectionResult> ScreenshotProtection::captureAndAnalyze() {
    std::vector<DetectionResult> results;
    auto overlays = detectOverlayApplications();
    for (auto& overlay : overlays) {
        results.push_back({0, "overlay-detection", overlay, "medium", "Potential suspicious overlay or screen capture tool detected", WinApiHelpers::currentTimestamp()});
    }
    if (results.empty()) {
        results.push_back({0, "screenshot-audit", "none", "low", "No suspicious overlays detected during screenshot protection sweep", WinApiHelpers::currentTimestamp()});
    }
    return results;
}
