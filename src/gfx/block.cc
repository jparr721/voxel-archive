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
                color = vec4(1, 1, 1, 1);
                break;
            case BlockType::kDirt:
                color = vec4(0.71, 0.61, 0.48, 1);
                break;
            case BlockType::kGrass:
                color = vec4(0.84, 0.92, 0.79, 1);
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
