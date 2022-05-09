#include "block.h"
#include "../util/colors.h"
#include <random>
#include <utility>

namespace vx::gfx {
    Block::Block(BlockType _blockType, BlockDir::BlockDirIndices _blockDirIndices, const vec3 &startingPosition)
        : blockType(_blockType), blockDirIndices(std::move(_blockDirIndices)) {
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

        if (startingPosition == vec3(0, 0, 0)) {
            for (const auto &vertex : kCubeVertices) { blockVertexColors.emplace_back(vertex, color); }
        } else {
            for (const auto &vertex : makeOffsetCubeVertices(startingPosition)) {
                blockVertexColors.emplace_back(vertex, color);
            }
        }
    }

    void translateBlock(const vec3 &diff, Block &block) {
        for (auto &[pos, _] : block.blockVertexColors) { pos += diff; }
    }

    auto makeOffsetCubeVertices(const vec3 &startingPosition) -> std::array<vec3, 8> {
        std::array<vec3, 8> vertices{};
        for (int ii = 0; ii < kCubeVertices.size(); ++ii) {
            const auto &vertex = kCubeVertices.at(ii);
            vertices.at(ii) = vertex + startingPosition;
        }
        return vertices;
    }
}// namespace vx::gfx