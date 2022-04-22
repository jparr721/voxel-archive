#include "key.h"

namespace vx::ctrl {
    auto createKeyMapping(const std::vector<Key::Enum> &keys) -> std::unordered_map<Key::Enum, bool> {
        std::unordered_map<Key::Enum, bool> keyMap;
        for (const auto &key : keys) { keyMap[key] = false; }
        return keyMap;
    }

    auto keyFromGLFWKey(u16 key) -> std::optional<Key> {
        switch (key) {
            default:
                return std::nullopt;
        }
    }
}// namespace vx::ctrl
