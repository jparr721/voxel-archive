#include "geometry.h"

namespace vx {
    auto sphericalToCartesian(float r, float theta, float phi) -> vec3 {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return {r * (cos_theta * sin_phi), r * cos_phi, r * (sin_theta * sin_phi)};
    }

    auto sphericalToCartesianDPhi(float r, float theta, float phi) -> vec3 {
        const auto sin_phi = std::sin(phi);
        const auto cos_phi = std::cos(phi);
        const auto sin_theta = std::sin(theta);
        const auto cos_theta = std::cos(theta);
        return {r * (cos_phi * cos_theta), -r * sin_phi, r * (cos_phi * sin_theta)};
    }
}// namespace vx
