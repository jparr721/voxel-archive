#include "key_input.h"
#include "../gfx/glfw.h"
#include <spdlog/spdlog.h>

namespace vx::ctrl {
    KeyInput *KeyInput::keyInput_ = nullptr;
    auto KeyInput::getInstance() -> KeyInput * {
        if (keyInput_ == nullptr) { keyInput_ = new KeyInput(); }
        return keyInput_;
    }

    void KeyInput::handleKeyPressEvent(int key, int scancode, int action, int mods) {
        // If the key is released, we don't want to double-activate the key. Alternatively, if we have no registered
        // callbacks, exit under that circumstance as well.
        if (action == GLFW_RELEASE || callbacks_.empty()) { return; }
        // If the key field is focused, we will add input to the string.
        for (const auto &[buf, state] : callbacks_) {
            // Only one thing should be focused at a time, so if we find it, jump out of this function.
            if (state.focused) {
                ImGui::GetIO().AddInputCharacter((unsigned int) key);
                return;
            }
        }
    }

    void KeyInput::registerImGuiKeyCallback(const std::string &name, std::string *buffer) {
        // Since we're using this with ImGui, we don't want to double-register, so this will silently
        // ignore repeated requests so that way we do not wipe the state
        if (doesKeyExist(name)) { return; }
        callbacks_.insert({name, KeyState(buffer)});
    }

    void KeyInput::removeImGuiKeyCallback(const std::string &name) {
        if (!doesKeyExist(name)) {
            spdlog::error("Failed to deregister callback {}", name);
            return;
        }
        callbacks_.erase(name);
    }

    auto KeyInput::handleImGuiKeyCallback(const std::string &name, ImGuiInputTextCallbackData *data) -> int {
        callbacks_.at(name).cursorPos = data->CursorPos;
        return 0;
    }

    void KeyInput::setLastKey(const std::string &name, u8 key) {
        if (!doesKeyExist(name)) {
            spdlog::error("Key not found {}", name);
            return;
        }
        callbacks_.at(name).lastKey = key;
    }

    void KeyInput::setCursorPos(const std::string &name, u8 cursorPos) {
        if (!doesKeyExist(name)) {
            spdlog::error("Key not found {}", name);
            return;
        }
        callbacks_.at(name).cursorPos = cursorPos;
    }

    void KeyInput::setFocused(const std::string &name, bool focused) {
        if (!doesKeyExist(name)) {
            spdlog::error("Key not found {}", name);
            return;
        }
        callbacks_.at(name).focused = focused;
    }

    auto KeyInput::doesKeyExist(const std::string &name) const -> bool const {
        return callbacks_.find(name) != callbacks_.end();
    }
}// namespace vx::ctrl
