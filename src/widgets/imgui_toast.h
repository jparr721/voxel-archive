#pragma once

#include "../math.h"

namespace vx::widgets {
    enum class ToastStatus {
        kStatusSuccess = 0x00,
        kStatusError = 0x01,
    };

    void showToastWidget(const char *message, u8 ttl, ToastStatus status);
    auto isToastWidgetVisible() -> bool &;
}// namespace vx::widgets
