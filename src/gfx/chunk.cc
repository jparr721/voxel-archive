#include "chunk.h"
#include <spdlog/spdlog.h>

namespace vx::gfx {
    Chunk::Chunk(const ivec3 &chunkSize) {
        spdlog::info("Loading chunk ({})", glm::to_string(chunkSize));
        // Origin point is always 0, 0, 0 so we draw from there
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    // TODO - Compute block direction
                    // TODO - Add custom color
                    // First block goes in first, all others go based on pos.
                    if (blocks.empty()) { blocks.emplace_back(BlockType::kDefault, BlockDir::kDebug); }

                    // Otherwise, make a new block, and increment the indices
                    BlockDir::BlockDirIndices indices = BlockDir::kDebug;
                    applyBlockDirOffset(blocks.size(), indices);
                    Block block(BlockType::kDefault, indices);
                    translateBlock(vec3(xx, yy, zz), block);
                    blocks.push_back(block);
                }
            }
        }
        spdlog::info("Chunk loaded");
    }

    auto Chunk::stacked() const -> std::pair<std::vector<VertexColorHex>, std::vector<u16>> {
        std::vector<VertexColorHex> geometry;
        std::vector<u16> indices;
        for (const auto &block : blocks) {
            geometry.insert(geometry.end(), block.blockVertexColors.begin(), block.blockVertexColors.end());
            indices.insert(indices.end(), block.blockDirIndices.begin(), block.blockDirIndices.end());
        }
        return {geometry, indices};
    }
}// namespace vx::gfx