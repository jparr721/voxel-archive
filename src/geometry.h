#pragma once

#include "math.h"

namespace vx {
    /**
   * glm::rotate for eigen.
   * @param m Matrix to multiply the rotation by
   * @param angle The rotation angle
   * @param axis The rotation axis, must be normalized
   */
    auto rotate(const mat4f &m, float angle, const vec3f &axis) -> mat4f;
    auto translate(const mat4f &m, const vec3f &translation) -> mat4f;
    auto perspectiveProjection(float near, float far, float fov, float aspect) -> mat4f;
    auto lookAt(const vec3f &eye, const vec3f &at, const vec3f &up) -> mat4f;
}// namespace vx
