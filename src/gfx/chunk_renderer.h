#pragma once

#include "../math.h"
#include <array>

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
}// namespace vx::gfx