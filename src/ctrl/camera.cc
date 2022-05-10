#include "camera.h"
#include "../geometry.h"
#include "../trigonometry.h"
#include "button.h"
#include <spdlog/spdlog.h>

namespace vx::ctrl {
    void Camera::reset() {
        displacement_ = vec3(0);
        eye_ = vec3(0, 0, 1);
        center_ = vec3(0);
        up_ = vec3(0, 1, 0);

        r_ = 1.0;
        theta_ = kPi / 2.0;
        phi_ = kPi / 2.0;
    }

    void Camera::resize(int width, int height) {
        aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
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
        const vec3 u_dir = leftDirection();
        const vec3 v_dir = downDirection();

        const vec3 u_disp = (du * pan_sensitivity) * u_dir;
        const vec3 v_disp = (dv * pan_sensitivity) * v_dir;
        const vec3 pan_disp = u_disp + v_disp;

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
        projection_matrix_ = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
    }

    void Camera::lockCamera(float minr, float maxr, float minTheta, float maxTheta, float minPhi, float maxPhi) {
        if (isLocked) {
            // Just fail silently for now.
            return;
        }

        // Notify that the camera is locked.
        isLocked = true;
        minr_ = minr;
        maxr_ = maxr;
        minTheta_ = minTheta;
        maxTheta_ = maxTheta;
        minPhi_ = minPhi;
        maxPhi_ = maxPhi;

        clampLockedParameters();
    }

    auto Camera::viewMatrix() -> mat4 & {
        compile();
        return view_matrix_;
    }

    auto Camera::projectionMatrix() -> mat4 & {
        compile();
        return projection_matrix_;
    }

    /// ==========================================
    void Camera::compile() {
        center_ = vec3(0.0f);
        eye_ = sphericalToCartesian(r_, theta_, phi_);
        up_ = glm::normalize(sphericalToCartesianDPhi(r_, theta_, phi_));

        // --------------------------------------------------------------------------------
        // Invert the up direction (since the spherical coordinates have phi
        // increasing downwards. Therefore we would like to have the (vector)
        // direction of the derivative inversed.
        // --------------------------------------------------------------------------------
        up_ *= -1.0;
        center_ += displacement_;
        eye_ += displacement_;
        view_matrix_ = glm::lookAt(eye_, center_, up_);

        if (isLocked) { clampLockedParameters(); }
    }

    void Camera::clampLockedParameters() {
        r_ = std::clamp(r_, minr_, maxr_);
        theta_ = std::clamp(theta_, minTheta_, maxTheta_);
        phi_ = std::clamp(phi_, minPhi_, maxPhi_);
    }

    auto Camera::leftDirection() -> vec3 {
        compile();
        const vec3 dir = glm::normalize(center_ - eye_);
        return glm::normalize(glm::cross(dir, up_));
    }

    auto Camera::downDirection() -> vec3 {
        compile();
        return -up_;
    }

    auto Camera::zoomAmount() const -> float { return r_; }
    auto Camera::theta() const -> float { return theta_; }
    auto Camera::phi() const -> float { return phi_; }
}// namespace vx::ctrl
