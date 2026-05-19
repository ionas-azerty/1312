#include "ProcessScanner.h"
#include "RuleBasedDetection.h"
#include "SignatureDetection.h"
#include "HeuristicDetection.h"
#include "BehaviorDetection.h"
#include "../../../shared/utils/StringUtils.h"
#include "../../../shared/utils/WinApiHelpers.h"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#endif

ProcessScanner::ProcessScanner(const AppConfig& config) : config_(config) {
    populateEngines();
}

void ProcessScanner::populateEngines() {
    engines_.emplace_back(std::make_unique<RuleBasedDetection>());
    engines_.emplace_back(std::make_unique<SignatureDetection>());
    engines_.emplace_back(std::make_unique<HeuristicDetection>());
    engines_.emplace_back(std::make_unique<BehaviorDetection>());
}

std::vector<ProcessInfo> ProcessScanner::enumerateProcesses() {
    std::vector<ProcessInfo> list;
#if defined(_WIN32) || defined(WIN32)
    DWORD processIds[1024];
    DWORD bytesReturned;
    if (EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
        size_t count = bytesReturned / sizeof(DWORD);
        for (size_t i = 0; i < count; ++i) {
            DWORD pid = processIds[i];
            if (pid == 0) continue;
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (!hProcess) continue;
            char name[MAX_PATH] = {0};
            GetModuleBaseNameA(hProcess, nullptr, name, MAX_PATH);
            char path[MAX_PATH] = {0};
            GetModuleFileNameExA(hProcess, nullptr, path, MAX_PATH);
            std::string windowClass = detectWindowClass(pid);
            std::vector<std::string> modules = collectModules(pid);
            list.push_back({static_cast<int>(pid), name, path, windowClass, modules});
            CloseHandle(hProcess);
        }
    }
#else
    // Fallback enumeration on non-Windows platforms.
    list.push_back({0, "vortex-backend", "./vortex-backend", "", {}});
#endif
    return list;
}

std::vector<std::string> ProcessScanner::collectModules(int pid) {
    std::vector<std::string> modules;
#if defined(_WIN32) || defined(WIN32)
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return modules;
    HMODULE moduleHandles[512];
    DWORD bytesNeeded;
    if (EnumProcessModules(hProcess, moduleHandles, sizeof(moduleHandles), &bytesNeeded)) {
        size_t count = bytesNeeded / sizeof(HMODULE);
        for (size_t i = 0; i < count; ++i) {
            char moduleName[MAX_PATH] = {0};
            GetModuleFileNameExA(hProcess, moduleHandles[i], moduleName, MAX_PATH);
            modules.emplace_back(moduleName);
        }
    }
    CloseHandle(hProcess);
#endif
    return modules;
}

std::string ProcessScanner::detectWindowClass(int pid) {
#if defined(_WIN32) || defined(WIN32)
    std::string foundClass;
    struct EnumData {
        int processId;
        std::string* target;
    } data{pid, &foundClass};
    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        auto* data = reinterpret_cast<EnumData*>(lParam);
        DWORD windowPid = 0;
        GetWindowThreadProcessId(hwnd, &windowPid);
        if (windowPid == static_cast<DWORD>(data->processId)) {
            char className[256] = {0};
            GetClassNameA(hwnd, className, sizeof(className));
            if (className[0] != '\0') {
                *data->target = className;
                return FALSE;
            }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&data));
    return foundClass;
#else
    return "";
#endif
}

std::vector<DetectionResult> ProcessScanner::scanProcesses() {
    std::vector<DetectionResult> results;
    auto processes = enumerateProcesses();
    for (auto& process : processes) {
        bool whitelisted = false;
        for (auto& item : config_.whitelist) {
            if (StringUtils::containsInsensitive(process.processName, item) || StringUtils::containsInsensitive(process.executablePath, item)) {
                whitelisted = true;
                break;
            }
        }
        if (whitelisted) continue;
        for (auto& engine : engines_) {
            auto engineResults = engine->detect(process);
            for (auto& entry : engineResults) {
                auto copy = entry;
                copy.timestamp = WinApiHelpers::currentTimestamp();
                results.push_back(copy);
            }
        }
    }
    return results;
}
