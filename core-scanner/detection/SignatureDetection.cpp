#include "SignatureDetection.h"
#include "../../../shared/utils/CryptoUtils.h"
#include "../../../shared/utils/StringUtils.h"

std::vector<DetectionResult> SignatureDetection::detect(const ProcessInfo& process) {
    std::vector<DetectionResult> results;
    const std::vector<uint32_t> suspiciousHashes = {
        CryptoUtils::simpleHash("cheatengine.exe"),
        CryptoUtils::simpleHash("x64dbg.exe"),
        CryptoUtils::simpleHash("processhacker.exe")
    };
    auto hash = CryptoUtils::simpleHash(StringUtils::toLower(process.processName));
    for (auto candidate : suspiciousHashes) {
        if (hash == candidate) {
            results.push_back({process.processId, process.processName, process.executablePath, "high", "Executable signature hash matches monitored tool", ""});
            break;
        }
    }
    return results;
}
