#pragma once

#include "bgfx.h"
#include "block.h"
#include "primitive.h"
#include <utility>
#include <vector>

namespace vx::gfx {
    struct Chunk {
        float minX;
        float maxX;
        float minY;
        float maxY;
        float minZ;
        float maxZ;

        std::vector<Block> blocks;
        std::vector<u16> indices;
        std::vector<VertexColorHex> geometry;

        Chunk(const ivec3 &chunkSize);
    };

    void translateChunk(const vec3 &amount, Chunk &chunk);
}// namespace vx::gfx