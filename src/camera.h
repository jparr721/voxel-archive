#pragma once

#include "geometry.h"
#include "math.h"
#include <Eigen/Dense>


namespace vx {
    class Camera {
    public:
        bool zooming = false;
        bool panning = false;
        bool rotating = false;

        float near_plane = 0.1;
        float far_plane = 1000.0;
        float rotation_sensitivity = 0.01;
        float pan_sensitivity = 0.01;
        float zoom_sensitivity = 0.1;
        float fovmin = 10.0;
        float fovmax = 120.0;
        float fov = 65.0;
        float radiusmin = 0.1;
        float radiusmax = 1000.0;
        float aspect_ratio = 4.0 / 3.0;

        Camera(float near_plane = 0.1, float far_plane = 1000.0, float rotation_sensitivity = 0.01,
               float pan_sensitivity = 0.01, float zoom_sensitivity = 0.1, float fovmin = 10.0, float fovmax = 120.0,
               float fov = 65.0, float radiusmin = 0.1, float radiusmax = 1000.0, float aspect_ratio = 4.0 / 3.0)
            : near_plane(near_plane), far_plane(far_plane), rotation_sensitivity(rotation_sensitivity),
              pan_sensitivity(pan_sensitivity), zoom_sensitivity(zoom_sensitivity), fovmin(fovmin), fovmax(fovmax),
              fov(fov), radiusmin(radiusmin), radiusmax(radiusmax), aspect_ratio(aspect_ratio) {}

        void reset();
        void resize(int width, int height);
        void zoom(float dr);
        void pan(float du, float dv);
        void rotate(float du, float dv);
        void setPerspective();

        auto viewMatrix() -> mat4f &;
        auto projectionMatrix() -> mat4f &;

    private:
        float r_ = 1.0;
        float theta_ = M_PI / 2.0;
        float phi_ = M_PI / 2.0;

        vec3f displacement_ = vec3f::Zero();
        vec3f eye_ = vec3f::Zero();
        vec3f center_ = vec3f::Zero();
        vec3f up_ = vec3f(0, 1, 0);

        mat4f view_matrix_ = mat4f::Identity();
        mat4f projection_matrix_ = mat4f::Identity();

        void compile();

        auto leftDirection() -> vec3f;
        auto downDirection() -> vec3f;
    };
}// namespace vx
