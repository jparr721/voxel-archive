#pragma once

#include "math.h"

namespace vx {
    /**
   * glm::rotate for eigen.
   * @param m Matrix to multiply the rotation by
   * @param angle The rotation angle
   * @param axis The rotation axis, must be normalized
   */
    auto rotate(const rmat4f &m, float angle, const vec3f &axis) -> rmat4f;
    auto translate(const rmat4f &m, const vec3f &translation) -> rmat4f;
    auto perspective(float fovy, float aspect, float near, float far) -> mat4f;
    auto lookAt(const vec3f &eye, const vec3f &center, const vec3f &up) -> mat4f;
}// namespace vx
