#include "camera.h"
namespace vx {

    void Camera::reset() {
        displacement_ = vec3f::Zero();
        eye_ = vec3f::Zero();
        center_ = vec3f::Zero();
        up_ = vec3f(0, 1, 0);

        r_ = 1.0;
        float theta_ = M_PI / 2.0;
        float phi_ = M_PI / 2.0;
    }

    void Camera::resize(int width, int height) {
        aspect_ratio = width / height;
        setPerspective();
        compile();
    }

    void Camera::zoom(float dr) {
        if (r_ + dr > radiusmax) {
            r_ = radiusmax;
            compile();
            return;
        }
        if (r_ + dr < radiusmin) {
            r_ = radiusmin;
            compile();
            return;
        }

        r_ += dr;
        compile();
    }

    void Camera::pan(float du, float dv) {
        const vec3f u_dir = leftDirection();
        const vec3f v_dir = downDirection();

        const vec3f u_disp = (du * pan_sensitivity) * u_dir;
        const vec3f v_disp = (du * pan_sensitivity) * v_dir;
        const vec3f pan_disp = u_disp + v_disp;

        displacement_ += pan_disp;
        compile();
    }

    void Camera::rotate(float du, float dv) {
        theta_ -= du * rotation_sensitivity;
        phi_ += dv * rotation_sensitivity;
        compile();
    }

    void Camera::setPerspective() {
        if (fov > fovmax) { fov = fovmax; }
        if (fov < fovmin) { fov = fovmin; }

        projection_matrix_ = perspective(near_plane, far_plane, fov, aspect_ratio);
    }

    auto Camera::viewMatrix() -> mat4f & {
        compile();
        return view_matrix_;
    }

    auto Camera::projectionMatrix() -> mat4f & {
        compile();
        return projection_matrix_;
    }

    /// ==========================================
    void Camera::compile() {
        /* center_ = vec3f::Zero(); */
        /* eye_ = sphericalToCartesian(r_, theta_, phi_); */
        /* up_ = sphericalToCartesianDPhi(r_, theta_, phi_).normalized(); */

        /* // -------------------------------------------------------------------------------- */
        /* // Invert the up direction (since the spherical coordinates have phi */
        /* // increasing downwards. Therefore we would like to have the (vector) */
        /* // direction of the derivative inversed. */
        /* // -------------------------------------------------------------------------------- */
        /* up_ *= -1.0; */
        /* center_ += displacement_; */
        /* eye_ += displacement_; */
        /* view_matrix_ = lookAt(eye_, center_, up_); */
    }

    auto Camera::leftDirection() -> vec3f {
        compile();
        const vec3f dir = (center_ - eye_).normalized();
        return dir.cross(up_).normalized();
    }

    auto Camera::downDirection() -> vec3f {
        compile();
        return -up_;
    }
}// namespace vx
