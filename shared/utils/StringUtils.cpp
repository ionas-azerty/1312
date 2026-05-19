#include "StringUtils.h"
#include <algorithm>

namespace StringUtils {
    std::string toLower(const std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    bool containsInsensitive(const std::string& value, const std::string& needle) {
        auto source = toLower(value);
        auto pattern = toLower(needle);
        return source.find(pattern) != std::string::npos;
    }
}
