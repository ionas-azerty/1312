#pragma once
#include <string>
#include <algorithm>

namespace StringUtils {
    std::string toLower(const std::string& value);
    bool containsInsensitive(const std::string& value, const std::string& needle);
}
