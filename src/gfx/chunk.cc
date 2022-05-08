#include "chunk.h"
#include <spdlog/spdlog.h>
#include <utility>

namespace vx::gfx {
    Chunk::Chunk(const ivec3 &chunkSize, std::string moduleName, std::string identifier)
        : shaderModule(moduleName), identifier(std::move(identifier)) {
        spdlog::debug("Loading chunk ({})", glm::to_string(chunkSize));
        // Origin point is always 0, 0, 0 so we draw from there
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    // TODO - Compute block direction
                    BlockDir::BlockDirIndices baseIndices = BlockDir::kDebug;
                    // TODO - Add custom color

                    // Increment indices to avoid overlapping faces
                    for (auto &index : baseIndices) { index += kCubeVertices.size() * blocks.size(); }

                    Block block(BlockType::kDebug, baseIndices);
                    translateBlock(vec3(xx, yy, zz), block);
                    blocks.push_back(block);

                    for (const auto &vertex : block.blockVertexColors) {
                        const auto x = vertex.position[0];
                        const auto y = vertex.position[1];
                        const auto z = vertex.position[2];

                        minX = std::min(minX, x);
                        maxX = std::max(maxX, x);

                        minY = std::min(minY, y);
                        maxY = std::max(maxY, y);

                        minZ = std::min(minZ, z);
                        maxZ = std::max(maxZ, z);

                        geometry.push_back(vertex);
                    }
                    for (const auto &index : block.blockDirIndices) { indices.push_back(index); }
                }
            }
        }
        spdlog::debug("Chunk loaded");
    }

    void translateChunk(const vec3 &amount, Chunk &chunk) {
        for (auto &[pos, _] : chunk.geometry) { pos += amount; }
    }
}// namespace vx::gfx