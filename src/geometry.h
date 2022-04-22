#pragma once

#include "math.h"

namespace vx {
    auto sphericalToCartesian(float r, float theta, float phi) -> vec3;
    auto sphericalToCartesianDPhi(float r, float theta, float phi) -> vec3;
}// namespace vx
