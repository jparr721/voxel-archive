#pragma once

#include "../math.h"
#include <imgui.h>
#include <memory>
#include <unordered_map>

namespace vx::ctrl {
    class KeyInput {
    public:
        void setFocused(const std::string &name, bool focused);
        void setCursorPos(const std::string &name, u8 cursorPos);
        void setLastKey(const std::string &name, u8 key);

        static auto getInstance() -> KeyInput *;

        void handleKeyPressEvent(int key, int scancode, int action, int mods);

        void registerImGuiKeyCallback(const std::string &name, std::string *buffer);
        void removeImGuiKeyCallback(const std::string &name);
        auto handleImGuiKeyCallback(const std::string &name, ImGuiInputTextCallbackData *data) -> int;

    private:
        struct KeyState {
            bool focused;
            u8 cursorPos;
            u8 lastKey;
            std::string *buffer;
            KeyState() = default;
            KeyState(std::string *buffer) : buffer(buffer) {}
        };


        std::string keyBuffer_;

        std::unordered_map<std::string, KeyState> callbacks_;

        static KeyInput *keyInput_;

        KeyInput() = default;
        KeyInput(const KeyInput &ki);
        auto operator=(const KeyInput &ki) -> KeyInput &;

        auto doesKeyExist(const std::string &name) const -> bool const;
    };
}// namespace vx::ctrl
