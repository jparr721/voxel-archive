#include "geometry.h"
#include <spdlog/spdlog.h>

namespace vx {

    auto rotate(const rmat4f &m, float angle, const vec3f &axis) -> rmat4f {
        const auto c = std::cos(angle);
        const auto s = std::sin(angle);

        vec3f temp = (1.0 - c) * axis;

        rmat4f rotation;
        rotation(0, 0) = c + temp(0) * axis(0);
        rotation(0, 1) = temp(0) * axis(1) + s * axis(2);
        rotation(0, 2) = temp(0) * axis(2) - s * axis(1);

        rotation(1, 0) = temp(1) * axis(0) - s * axis(2);
        rotation(1, 1) = c + temp(1) * axis(1);
        rotation(1, 2) = temp(1) * axis(2) + s * axis(0);

        rotation(2, 0) = temp(2) * axis(0) + s * axis(1);
        rotation(2, 1) = temp(2) * axis(1) - s * axis(0);
        rotation(2, 2) = c + temp(2) * axis(2);

        rmat4f ret;
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
    auto translate(const rmat4f &m, const vec3f &translation) -> rmat4f {
        rmat4f ret = m;
        ret.row(3) = m.row(0) * translation(0) + m.row(1) * translation(1) + m.row(2) * translation(2) + m.row(3);
        return ret;
    }

    auto perspective(float fovy, float aspect, float near, float far) -> mat4f {
        assert(abs(aspect - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));

        float const tanHalfFovy = tan(fovy / static_cast<float>(2));

        mat4f result = mat4f::Zero();
        result(0, 0) = static_cast<float>(1) / (aspect * tanHalfFovy);
        result(1, 1) = static_cast<float>(1) / (tanHalfFovy);
        result(2, 2) = -(far + near) / (far - near);
        result(2, 3) = -static_cast<float>(1);
        result(3, 2) = -(static_cast<float>(2) * far * near) / (far - near);
        return result.transpose();
    }

    auto lookAt(const vec3f &eye, const vec3f &center, const vec3f &up) -> mat4f {
        const vec3f f = (center - eye).normalized();
        const vec3f s = f.cross(up).normalized();
        const vec3f u = s.cross(f);

        mat4f result = mat4f::Identity();
        result(0, 0) = s.x();
        result(1, 0) = s.y();
        result(2, 0) = s.z();
        result(0, 1) = u.x();
        result(1, 1) = u.y();
        result(2, 1) = u.z();
        result(0, 2) = -f.x();
        result(1, 2) = -f.y();
        result(2, 2) = -f.z();
        result(3, 0) = -s.dot(eye);
        result(3, 1) = -u.dot(eye);
        result(3, 2) = f.dot(eye);
        return result.transpose();
    }

}// namespace vx
