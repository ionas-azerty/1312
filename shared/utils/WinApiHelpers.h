#pragma once
#include <string>
#include <vector>
#include <chrono>
#if defined(_WIN32) || defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#endif

namespace WinApiHelpers {
    std::string currentTimestamp();
    bool isProcessSigned(const std::string& path);
    bool isDebuggerActive();
    bool hasVirtualMachineArtifacts();
}
