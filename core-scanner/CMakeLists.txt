add_library(vortex-scanner STATIC
    detection/IDetectionEngine.h
    detection/RuleBasedDetection.cpp
    detection/SignatureDetection.cpp
    detection/HeuristicDetection.cpp
    detection/BehaviorDetection.cpp
    scanner/ProcessScanner.cpp
    scanner/MemoryModuleMonitor.cpp
    scanner/ScreenshotProtection.cpp
    scanner/SystemProtection.cpp
)
target_include_directories(vortex-scanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/shared)
if (WIN32)
    target_compile_definitions(vortex-scanner PRIVATE PLATFORM_WINDOWS)
endif()
