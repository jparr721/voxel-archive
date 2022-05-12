#include "chunk.h"
#include "../paths.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <utility>

namespace vx::gfx {
    Chunk::Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation, std::string moduleName, std::string _identifier)
        : shaderModule(std::move(moduleName)), identifier(std::move(_identifier)) {
        spdlog::debug("Loading chunk id: {} module: {}", identifier, shaderModule);
        // Origin point is always 0 0 0, so we draw from there
        u64 ii = 0;
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    // TODO - Compute block direction
                    BlockDir::BlockDirIndices baseIndices = BlockDir::kDebug;
                    // TODO - Add custom color

                    // Increment indices to avoid overlapping faces
                    for (auto &index : baseIndices) {
                        index += kCubeVertices.size() * ii;
                        indices.push_back(index);
                    }

                    const u32 color = makeColorFromBlockType(BlockType::kDebug);
                    const vec3 startingPosition = vec3(xx, yy, zz);
                    for (const auto &vertex : makeOffsetCubeVertices(startingPosition)) {
                        geometry.emplace_back(vertex, color);
                    }
                    ++ii;
                }
            }
        }

        // Translate the chunk
        for (auto &[pos, _] : geometry) { pos += chunkTranslation; }
        setBounds();
        spdlog::debug("Chunk loaded successfully");
    }

    void Chunk::write(bool isFixture) const noexcept {
        spdlog::info("IDENTIFIER {}", identifier);
        const fs::path filepath = isFixture ? paths::kFixturesPath / fs::path(identifier + ".chunk")
                                            : paths::kAssetsPath / fs::path(identifier + ".chunk");
        spdlog::info("Writing chunk to {}", filepath.string());
        std::ofstream chunkfile(filepath);

        if (!chunkfile.is_open()) {
            spdlog::error("chunkfile failed to open, exiting write operation");
            return;
        }

        chunkfile << "identifier " << identifier << std::endl;
        chunkfile << "shader module " << shaderModule << std::endl;

        chunkfile << "minx " << minX << std::endl;
        chunkfile << "maxx " << maxX << std::endl;

        chunkfile << "miny " << minY << std::endl;
        chunkfile << "maxy " << maxY << std::endl;

        chunkfile << "minz " << minZ << std::endl;
        chunkfile << "maxz " << maxZ << std::endl;

        for (const auto &index : indices) { chunkfile << "i " << index << std::endl; }
        for (const auto &vertexColorHex : geometry) {
            chunkfile << "ic " << vertexColorHex.position[0] << " " << vertexColorHex.position[1] << " "
                      << vertexColorHex.position[2] << " " << vertexColorHex.color << std::endl;
        }
        chunkfile.close();
    }

    auto Chunk::operator==(const gfx::Chunk &other) const -> bool {
        return identifier == other.identifier && shaderModule == other.shaderModule && minX == other.minX &&
               maxX == other.maxX && minY == other.minY && maxY == other.maxY && minZ == other.minZ &&
               maxZ == other.maxZ && indices == other.indices && geometry == other.geometry;
    }

    void Chunk::setBounds() {
        for (const auto &vertex : geometry) {
            const auto x = vertex.position[0];
            const auto y = vertex.position[1];
            const auto z = vertex.position[2];

            minX = std::min(minX, x);
            maxX = std::max(maxX, x);

            minY = std::min(minY, y);
            maxY = std::max(maxY, y);

            minZ = std::min(minZ, z);
            maxZ = std::max(maxZ, z);
        }
    }
}// namespace vx::gfx
