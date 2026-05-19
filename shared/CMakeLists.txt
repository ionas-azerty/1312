add_library(vortex-shared STATIC
    utils/FileUtils.cpp
    utils/StringUtils.cpp
    utils/CryptoUtils.cpp
    utils/WinApiHelpers.cpp
    types/AppConfig.h
    types/LogEntry.h
    types/ProcessInfo.h
)
target_include_directories(vortex-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (WIN32)
    target_compile_definitions(vortex-shared PRIVATE PLATFORM_WINDOWS)
endif()
