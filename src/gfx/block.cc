#include "block.h"
#include <random>
#include <spdlog/spdlog.h>

namespace vx::gfx {
    static constexpr auto BlockTypeStringkDefault = "kDefault";
    static constexpr auto BlockTypeStringkDirt = "kDirt";
    static constexpr auto BlockTypeStringkGrass = "kGrass";
    static constexpr auto BlockTypeStringkDebug = "kDebug";

    auto makeOffsetCubeVertices(const vec3 &startingPosition) -> std::array<vec3, 8> {
        std::array<vec3, 8> vertices{};
        for (int ii = 0; ii < kCubeVertices.size(); ++ii) {
            const auto &vertex = kCubeVertices.at(ii);
            vertices.at(ii) = vertex + startingPosition;
        }
        return vertices;
    }

    auto makeColorFromBlockType(BlockType blockType) -> vec4 {
        vec4 color;
        switch (blockType) {
            case BlockType::kDefault:
                color = util::u32ToRgba(0xffffffff);
                break;
            case BlockType::kDirt:
                color = util::u32ToRgba(0xff000000);
                break;
            case BlockType::kGrass:
                color = util::u32ToRgba(0xffff0000);
                break;
            case BlockType::kDebug:
                std::random_device r;
                std::mt19937 engine(r());
                std::uniform_real_distribution<f32> distr(0, 1);
                color = vec4(distr(engine), distr(engine), distr(engine), distr(engine));
                break;
        }

        return color;
    }

    auto blockTypeToString(BlockType blockType) -> std::string {
        switch (blockType) {
            case BlockType::kDefault:
                return BlockTypeStringkDefault;
                break;
            case BlockType::kDirt:
                return BlockTypeStringkDirt;
                break;
            case BlockType::kGrass:
                return BlockTypeStringkGrass;
                break;
            case BlockType::kDebug:
                return BlockTypeStringkDebug;
                break;
        }
    }

    auto stringToBlockType(const std::string &blockType) -> BlockType {
        if (blockType == BlockTypeStringkDefault) {
            return BlockType::kDefault;
        } else if (blockType == BlockTypeStringkDirt) {
            return BlockType::kDirt;
        } else if (blockType == BlockTypeStringkGrass) {
            return BlockType::kGrass;
        }
        return BlockType::kDebug;
    }
}// namespace vx::gfx
