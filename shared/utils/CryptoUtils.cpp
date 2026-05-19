#include "CryptoUtils.h"

namespace CryptoUtils {
    uint32_t simpleHash(const std::string& value) {
        uint32_t hash = 2166136261u;
        for (unsigned char c : value) {
            hash ^= c;
            hash *= 16777619u;
        }
        return hash;
    }
}
