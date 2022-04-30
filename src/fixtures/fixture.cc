#include "fixture.h"

namespace vx::fixtures {
    static Fixture baseLayerFixture;

    auto initializeFixture(std::vector<gfx::Chunk> &chunks) -> Fixture {
        Fixture fixture;
        fixture.renderer = std::make_unique<gfx::ChunkRenderer>(chunks);
        fixture.chunks = chunks;
        return fixture;
    }

    auto getBaseLayerFixture() -> Fixture & {
        static std::vector<gfx::Chunk> baseLayerChunks{
                ivec3(50, 1, 50),
                ivec3(50, 1, 50),
                ivec3(50, 1, 50),
                ivec3(50, 1, 50),
        };

        {
            auto &chunk = baseLayerChunks.at(1);
            const auto translationAmount = vec3(50, 0, 0);
            gfx::translateChunk(translationAmount, chunk);
        }

        {
            auto &chunk = baseLayerChunks.at(2);
            const auto translationAmount = vec3(0, 0, 50);
            gfx::translateChunk(translationAmount, chunk);
        }
        {
            auto &chunk = baseLayerChunks.at(3);
            const auto translationAmount = vec3(50, 0, 50);
            gfx::translateChunk(translationAmount, chunk);
        }

        baseLayerFixture = initializeFixture(baseLayerChunks);
        return baseLayerFixture;
    }
}// namespace vx::fixtures