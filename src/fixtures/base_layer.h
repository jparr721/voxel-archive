#pragma once

#include "../gfx/chunk.h"
#include "fixture.h"
#include <memory>

namespace vx::fixtures {
    static gfx::Chunk q1(ivec3(50, 1, 50));
    static gfx::Chunk q2(ivec3(50, 1, 50));
    static gfx::Chunk q3(ivec3(50, 1, 50));
    static gfx::Chunk q4(ivec3(50, 1, 50));

    static std::vector<gfx::Chunk> chunks{q1, q2, q3, q4};
    static std::unique_ptr<Fixture<2, 2>> baseLayerFixture = std::make_unique<Fixture<2, 2>>(chunks);
}// namespace vx::fixtures