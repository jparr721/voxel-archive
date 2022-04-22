#pragma once

#include "../math.h"
#include <optional>
#include <unordered_map>
#include <vector>

namespace vx::ctrl {
    struct Key {
        enum Enum {
            kNone = 0,
            kEsc,
            kReturn,
            kTab,
            kSpace,
            kBackspace,
            kUp,
            kDown,
            kLeft,
            kRight,
            kInsert,
            kDelete,
            kHome,
            kEnd,
            kPageUp,
            kPageDown,
            kPrint,
            kPlus,
            kMinus,
            kLeftBracket,
            kRightBracket,
            kSemicolon,
            kQuote,
            kComma,
            kPeriod,
            kSlash,
            kBackslash,
            kTilde,
            kF1,
            kF2,
            kF3,
            kF4,
            kF5,
            kF6,
            kF7,
            kF8,
            kF9,
            kF10,
            kF11,
            kF12,
            kNumPad0,
            kNumPad1,
            kNumPad2,
            kNumPad3,
            kNumPad4,
            kNumPad5,
            kNumPad6,
            kNumPad7,
            kNumPad8,
            kNumPad9,
            kKey0,
            kKey1,
            kKey2,
            kKey3,
            kKey4,
            kKey5,
            kKey6,
            kKey7,
            kKey8,
            kKey9,
            kKeyA,
            kKeyB,
            kKeyC,
            kKeyD,
            kKeyE,
            kKeyF,
            kKeyG,
            kKeyH,
            kKeyI,
            kKeyJ,
            kKeyK,
            kKeyL,
            kKeyM,
            kKeyN,
            kKeyO,
            kKeyP,
            kKeyQ,
            kKeyR,
            kKeyS,
            kKeyT,
            kKeyU,
            kKeyV,
            kKeyW,
            kKeyX,
            kKeyY,
            kKeyZ,

            kCount
        };
    };

    auto createKeyMapping(const std::vector<Key::Enum> &keys) -> std::unordered_map<Key::Enum, bool>;
    auto keyFromGLFWKey(u16 key) -> std::optional<Key>;

    struct Modifier {
        enum Enum {
            kNone = 0,
            kLeftAlt,
            kRightAlt,
            kLeftCtrl,
            kRightCtrl,
            kLeftShift,
            kRightShift,
            kLeftMeta,
            kRightMeta,
            kCount
        };
    };
}// namespace vx::ctrl
