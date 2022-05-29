#include "imgui_toast.h"
#include "../util/timer.h"
#include "../window.h"
#include "imgui.h"
#include <chrono>
#include <memory>
#include <spdlog/spdlog.h>

namespace vx::widgets {
    constexpr int kToastWidgetWidth = 140;
    constexpr int kToastWidgetHeight = 40;
    constexpr int kRightPadding = 40;
    constexpr int kBottomPadding = 100;

    auto assignTextColor(ToastStatus status) -> ImVec4 {
        switch (status) {
            case ToastStatus::kStatusError:
                return ImVec4(1, 0, 0, 1);
                break;
            case ToastStatus::kStatusSuccess:
                return ImVec4(0, 1, 0, 1);
                break;
        }

        return ImVec4(1, 1, 1, 1);
    }

    bool toastVisible = false;
    std::unique_ptr<util::Timer> timer = nullptr;

    void showToastWidget(const char *message, u8 ttl, ToastStatus status) {
        const auto windowSize = currentWindowSize();

        const auto windowWidthOffset = windowSize.x - kToastWidgetWidth - kRightPadding;
        const auto windowHeightOffset = windowSize.y - kToastWidgetHeight - kBottomPadding;

        // Map Position to Bottom Right
        ImGui::SetNextWindowSize(ImVec2(kToastWidgetWidth, kToastWidgetHeight));
        ImGui::SetNextWindowPos(ImVec2(windowWidthOffset, windowHeightOffset));

        // Show the content
        if (toastVisible) {
            if (timer == nullptr) {
                timer = std::make_unique<util::Timer>();
                timer->start();
            }

            ImGui::Begin("Alert", nullptr,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            ImGui::TextColored(assignTextColor(status), "%s", message);
            ImGui::End();

            if (timer->elapsed() >= ttl) {
                toastVisible = false;
                timer = nullptr;
            }
        }
    }

    auto isToastWidgetVisible() -> bool & { return toastVisible; }
}// namespace vx::widgets
