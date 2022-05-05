#pragma once

#include "../math.h"
#include "bgfx.h"
#include "primitive.h"
#include <array>
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

    // The four unique vertices per face
    static constexpr std::array<usize, 4> kUniqueIndices{0, 1, 2, 5};

    // The two indicies per cube face
    static constexpr std::array<usize, 6> kFaceIndices{0, 1, 2, 0, 2, 3};

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

    enum class BlockType { kDefault = 0, kGrass, kDirt, kDebug };

    struct BlockDir {
        using BlockDirIndices = std::vector<u16>;
        // The indices for each direction
        inline static const BlockDirIndices kSouth{4, 7, 6, 4, 6, 5};// (south (+z))
        inline static const BlockDirIndices kNorth{3, 0, 1, 3, 1, 2};// (north (-z))
        inline static const BlockDirIndices kEast{7, 3, 2, 7, 2, 6}; // (east  (+x))
        inline static const BlockDirIndices kWest{0, 4, 5, 0, 5, 1}; // (west  (-x))
        inline static const BlockDirIndices kUp{2, 1, 5, 2, 5, 6};   // (up    (+y))
        inline static const BlockDirIndices kDown{0, 3, 7, 0, 7, 4}; // (down  (-y))
        inline static const BlockDirIndices kDebug{
                4, 7, 6, 4, 6, 5,// South
                3, 0, 1, 3, 1, 2,// North
                7, 3, 2, 7, 2, 6,// East
                0, 4, 5, 0, 5, 1,// West
                2, 1, 5, 2, 5, 6,// Up
                0, 3, 7, 0, 7, 4,// Down
        };
    };
    void applyBlockDirOffset(u64 amount, BlockDir::BlockDirIndices &blockDirIndices);

    struct Block {
        bool active;
        bool selected;

        BlockType blockType;
        BlockDir::BlockDirIndices blockDirIndices;
        std::vector<gfx::VertexColorHex> blockVertexColors;

        Block(BlockType _blockType, BlockDir::BlockDirIndices _blockDirIndices);
        auto indexSize() -> u32 { return blockDirIndices.size() * sizeof(u16); }
    };

    void translateBlock(const vec3 &diff, Block &block);
}// namespace vx::gfx