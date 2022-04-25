#pragma once

#include "bgfx.h"
#include "block.h"
#include <vector>

namespace vx::gfx {
    struct Chunk {
        std::vector<Block> blocks;
    };
}// namespace vx::gfx