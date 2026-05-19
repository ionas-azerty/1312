#include "WinApiHelpers.h"
#include <array>
#include <ctime>

namespace WinApiHelpers {
    std::string currentTimestamp() {
        using clock = std::chrono::system_clock;
        auto now = clock::to_time_t(clock::now());
        std::tm tm;
#if defined(_WIN32) || defined(WIN32)
        localtime_s(&tm, &now);
#else
        localtime_r(&now, &tm);
#endif
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
        return std::string(buffer);
    }

    bool isProcessSigned(const std::string& /*path*/) {
        return false;
    }

    bool isDebuggerActive() {
#if defined(_WIN32) || defined(WIN32)
        return IsDebuggerPresent() != 0;
#else
        return false;
#endif
    }

    bool hasVirtualMachineArtifacts() {
#if defined(_WIN32) || defined(WIN32)
        std::array<std::string, 4> knownNames = {"vmware", "virtualbox", "vbox", "qemu"};
        DWORD size = 256;
        char buffer[256];
        if (GetComputerNameA(buffer, &size) == 0) return false;
        std::string name = buffer;
        for (auto& token : knownNames) {
            if (name.find(token) != std::string::npos) return true;
        }
#endif
        return false;
    }
}
