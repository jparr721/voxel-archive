#include "block.h"
#include <random>

namespace vx::gfx {
    auto makeOffsetCubeVertices(const vec3 &startingPosition) -> std::array<vec3, 8> {
        std::array<vec3, 8> vertices{};
        for (int ii = 0; ii < kCubeVertices.size(); ++ii) {
            const auto &vertex = kCubeVertices.at(ii);
            vertices.at(ii) = vertex + startingPosition;
        }
        return vertices;
    }

    auto makeColorFromBlockType(BlockType blockType) -> u32 {
        u32 color;
        switch (blockType) {
            case BlockType::kDefault:
                color = 0xffffffff;
                break;
            case BlockType::kDirt:
                color = 0xff000000;
                break;
            case BlockType::kGrass:
                color = 0xffff0000;
                break;
            case BlockType::kDebug:
                std::random_device r;
                std::mt19937 engine(r());
                std::uniform_int_distribution<u32> distr(0, 255);
                vec4 cv(distr(engine), distr(engine), distr(engine), distr(engine));
                color = util::rgbaToU32(cv);
                break;
        }
        return color;
    }
}// namespace vx::gfx