#include "geometry.h"
#include <spdlog/spdlog.h>

namespace vx {
    auto rotate(const mat4f &m, float angle, const vec3f &axis) -> mat4f {
        const auto c = std::cos(angle);
        const auto s = std::sin(angle);

        vec3f temp = (1.0 - c) * axis;

        mat4f rotation;
        rotation(0, 0) = c + temp(0) * axis(0);
        rotation(0, 1) = temp(0) * axis(1) + s * axis(2);
        rotation(0, 2) = temp(0) * axis(2) - s * axis(1);

        rotation(1, 0) = temp(1) * axis(0) - s * axis(2);
        rotation(1, 1) = c + temp(1) * axis(1);
        rotation(1, 2) = temp(1) * axis(2) + s * axis(0);

        rotation(2, 0) = temp(2) * axis(0) + s * axis(1);
        rotation(2, 1) = temp(2) * axis(1) - s * axis(0);
        rotation(2, 2) = c + temp(2) * axis(2);

        mat4f ret;
        ret.row(0) = m.row(0) * rotation(0, 0) + m.row(1) * rotation(0, 1) + m.row(2) * rotation(0, 2);
        ret.row(1) = m.row(0) * rotation(1, 0) + m.row(1) * rotation(1, 1) + m.row(2) * rotation(1, 2);
        ret.row(2) = m.row(0) * rotation(2, 0) + m.row(1) * rotation(2, 1) + m.row(2) * rotation(2, 2);
        ret.row(3) = m.row(3);
        return ret;
    }

    /**
     * glm::translate for eigen containers. Builds a translation 4x4 matrix from a 3-component vector
     * @param m The matrix to multiply by this translation
     * @param translation The coordinates of the translation vector
     */
    auto translate(const mat4f &m, const vec3f &translation) -> mat4f {
        mat4f ret;
        ret.row(3) = m.row(0) * translation(0) + m.row(1) * translation(1) + m.row(2) * translation(2) + m.row(3);
        return ret;
    }

    auto perspectiveProjection(float near, float far, float fov, float aspect) -> mat4f {
        const auto ymax = near * std::tan(fov * M_PI / 360.0);
        const auto xmax = ymax * aspect;

        const auto left = -xmax;
        const auto right = xmax;
        const auto bottom = -ymax;
        const auto top = ymax;

        const auto A = (right + left) / (right - left);
        const auto B = (top + bottom) / (top - bottom);
        const auto C = -(far + near) / (far - near);
        const auto D = -2.0 * far * near / (far - near);
        const auto E = 2.0 * near / (right - left);
        const auto F = 2.0 * near / (top - bottom);

        mat4f ret = mat4f::Zero();

        ret(0, 0) = E;

        ret(1, 1) = F;

        ret(2, 0) = A;
        ret(2, 1) = B;
        ret(2, 2) = C;
        ret(2, 3) = -1.0;

        ret(3, 2) = D;
        return ret;
    }

    auto lookAt(const vec3f &eye, const vec3f &at, const vec3f &up) -> mat4f {
        const vec3f forward = (at - eye).normalized();
        const vec3f side = forward.cross(up).normalized();
        const vec3f up_ = side.cross(forward).normalized();

        mat4f ret = mat4f::Zero();

        ret.row(0) << side(0), up_(0), -forward(0), 0.0;
        ret.row(1) << side(1), up_(1), -forward(1), 0.0;
        ret.row(2) << side(2), up_(2), -forward(2), 0.0;
        ret.row(3) << -side.dot(eye), -up.dot(eye), forward.dot(eye), 1.0;

        return ret;
    }

}// namespace vx
