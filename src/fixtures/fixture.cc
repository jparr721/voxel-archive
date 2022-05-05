#include "fixture.h"

namespace vx::fixtures {
    static std::shared_ptr<Fixture> baseLayerFixture = nullptr;

    auto initializeFixture(std::vector<gfx::Chunk> &chunks) -> std::shared_ptr<Fixture> {
        std::shared_ptr<Fixture> fixture = std::make_shared<Fixture>();
        fixture->renderer = std::make_unique<gfx::ChunkRenderer>(chunks);
        fixture->chunks = chunks;
        return fixture;
    }

    auto getBaseLayerFixture() -> std::shared_ptr<Fixture> {
        if (baseLayerFixture == nullptr) {
            static std::vector<gfx::Chunk> baseLayerChunks;
            baseLayerChunks.emplace_back(ivec3(50, 1, 50), "Chunk 1");
            baseLayerChunks.emplace_back(ivec3(50, 1, 50), "Chunk 2");
            baseLayerChunks.emplace_back(ivec3(50, 1, 50), "Chunk 3");
            baseLayerChunks.emplace_back(ivec3(50, 1, 50), "Chunk 4");

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
        }
        return baseLayerFixture;
    }
}// namespace vx::fixtures