#pragma once

#include "../math.h"
#include "../util/strings.h"
#include <imgui.h>
#include <memory>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace vx::ctrl {
    class KeyInput {
    private:
        struct KeyState {
            bool focused = false;
            u8 lastKey = 0;
            std::string buffer;
            KeyState() = default;
            friend auto operator<<(std::ostream &os, const KeyState &ks) -> std::ostream & {
                std::stringstream ss;
                ss << "KeyState(focused=" << ks.focused << ", lastKey=" << ks.lastKey << ", buffer=" << ks.buffer
                   << ")";
                os << ss.str();
                return os;
            }
        };

    public:
        void setFocused(const std::string &name, bool focused);
        void setLastKey(const std::string &name, u8 key);

        static auto getInstance() -> KeyInput *;

        void handleKeyPressEvent(int key, int scancode, int action, int mods);

        void registerImGuiKeyCallback(const std::string &name);
        void removeImGuiKeyCallback(const std::string &name);
        auto getCallbackState(const std::string &name) -> std::optional<KeyState>;

    private:
        std::unordered_map<std::string, KeyState> callbacks_;

        static KeyInput *keyInput_;

        KeyInput() = default;
        KeyInput(const KeyInput &ki);
        auto operator=(const KeyInput &ki) -> KeyInput &;

        auto doesKeyExist(const std::string &name) const -> bool;
    };
}// namespace vx::ctrl
