#pragma once

#include "../math.h"
#include "../util/colors.h"
#include "bgfx.h"
#include "primitive.h"
#include <array>
#include <random>
#include <vector>

namespace vx::gfx {
    /*  3D CUBE
    *  1-------2
    *  |5------+6
    *  ||      ||
    *  ||      ||
    *  0+------3|
    *   4-------7
    *
    *  bottom:
    *  4-------7
    *  |       |
    *  |       |
    *  |       |
    *  0-------3
    *
    * top:
    *  5-------6
    *  |       |
    *  |       |
    *  |       |
    *  1-------2
    *
    * vertices:
    *  (0, 0, 0)
    *  (0, 1, 0)
    *  (1, 1, 0)
    *  (1, 0, 0)
    *  (0, 0, 1)
    *  (0, 1, 1)
    *  (1, 1, 1)
    *  (1, 0, 1)
    *
    * indices:
    * 4, 7, 6, 4, 6, 5, // (south (+z))
    * 3, 0, 1, 3, 1, 2, // (north (-z))
    * 7, 3, 2, 7, 2, 6, // (east  (+x))
    * 0, 4, 5, 0, 5, 1, // (west  (-x))
    * 2, 1, 5, 2, 5, 6, // (up    (+y))
    * 0, 3, 7, 0, 7, 4  // (down  (-y))
    */

    static constexpr std::array<vec3, 8> kCubeVertices{vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0),
                                                       vec3(0, 0, 1), vec3(0, 1, 1), vec3(1, 1, 1), vec3(1, 0, 1)};

    static constexpr std::array<vec3, 6> kCubeNormals{
            vec3(0, 0, 1), vec3(0, 0, -1), vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0, 1, 0), vec3(0, -1, 0),
    };

    static constexpr std::array<vec2, 4> kCubeUVs{
            vec2(0, 0),
            vec2(1, 0),
            vec2(1, 1),
            vec2(0, 1),
    };

    using BlockIndexSize = u32;

    enum class BlockType { kDefault = 0, kGrass, kDirt, kDebug };

    inline static const std::array<BlockIndexSize, 36> kBlockIndices = {
            4, 7, 6, 4, 6, 5,// South
            3, 0, 1, 3, 1, 2,// North
            7, 3, 2, 7, 2, 6,// East
            0, 4, 5, 0, 5, 1,// West
            2, 1, 5, 2, 5, 6,// Up
            0, 3, 7, 0, 7, 4,// Down
    };

    auto makeOffsetCubeVertices(const vec3 &startingPosition) -> std::array<vec3, 8>;
    auto makeColorFromBlockType(BlockType blockType) -> vec4;
    auto blockTypeToString(BlockType blockType) -> std::string;
    auto stringToBlockType(const std::string &blockType) -> BlockType;
}// namespace vx::gfx
