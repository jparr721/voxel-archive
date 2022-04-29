#pragma once

#include "../gfx/chunk_renderer.h"
#include <vector>

namespace vx::fixtures {
    template<unsigned LayoutRows, unsigned LayoutCols>
    class Fixture {
    public:
        Fixture(const std::vector<gfx::Chunk> &chunks) : chunks_(chunks) {
            // Assign the layout to the chunk
            {
                auto &chunk = chunks_.at(1);
                const auto translationAmount = vec3(50, 0, 0);
                gfx::translateChunk(translationAmount, chunk);
            }

            {
                auto &chunk = chunks_.at(2);
                const auto translationAmount = vec3(0, 0, 50);
                gfx::translateChunk(translationAmount, chunk);
            }
            {
                auto &chunk = chunks_.at(3);
                const auto translationAmount = vec3(50, 0, 50);
                gfx::translateChunk(translationAmount, chunk);
            }
        }

        auto renderer() -> const std::unique_ptr<gfx::ChunkRenderer> & {
            if (chunkRenderer_ == nullptr) { chunkRenderer_ = std::make_unique<gfx::ChunkRenderer>(chunks_); }
            return chunkRenderer_;
        }

    private:
        std::unique_ptr<gfx::ChunkRenderer> chunkRenderer_;
        std::vector<gfx::Chunk> chunks_;
    };
}// namespace vx::fixtures