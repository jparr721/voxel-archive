#include "chunk.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace vx::gfx {

    Chunk::Chunk(const ivec3 &chunkSize) {
        spdlog::debug("Loading chunk ({})", glm::to_string(chunkSize));
        // Origin point is always 0, 0, 0 so we draw from there
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    BlockDir::BlockDirIndices baseIndices = BlockDir::kDebug;
                    // TODO - Compute block direction
                    // TODO - Add custom color

                    // Increment indices to avoid overlapping faces
                    for (auto &index : baseIndices) { index += kCubeVertices.size() * blocks.size(); }

                    Block block(BlockType::kRandom, baseIndices);
                    translateBlock(vec3(xx, yy, zz), block);
                    blocks.push_back(block);

                    for (const auto &vertex : block.blockVertexColors) { geometry.push_back(vertex); }
                    for (const auto &index : block.blockDirIndices) { indices.push_back(index); }
                }
            }
        }
        spdlog::debug("Chunk loaded");
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