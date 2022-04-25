#pragma once

#include "bgfx.h"
#include "block.h"
#include "primitive.h"
#include <utility>
#include <vector>

namespace vx::gfx {
    struct Chunk {
        std::vector<Block> blocks;

        Chunk(const ivec3 &chunkSize);
        auto stacked() const -> std::pair<std::vector<VertexColorHex>, std::vector<u16>>;
    };
}// namespace vx::gfx