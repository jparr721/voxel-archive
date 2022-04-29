#pragma once

#include "../math.h"
#include "button.h"
#include "key.h"
#include <array>
#include <unordered_map>


namespace vx::ctrl {
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

        explicit Camera(float near_plane = 0.1, float far_plane = 1000.0, float rotation_sensitivity = 0.01,
                        float pan_sensitivity = 0.01, float zoom_sensitivity = 0.1, float fovmin = 10.0,
                        float fovmax = 120.0, float fov = 120.0, float radiusmin = 0.1, float radiusmax = 1000.0,
                        float aspect_ratio = 4.0 / 3.0)
            : near_plane(near_plane), far_plane(far_plane), rotation_sensitivity(rotation_sensitivity),
              pan_sensitivity(pan_sensitivity), zoom_sensitivity(zoom_sensitivity), fovmin(fovmin), fovmax(fovmax),
              fov(fov), radiusmin(radiusmin), radiusmax(radiusmax), aspect_ratio(aspect_ratio) {}

        void reset();
        void resize(int width, int height);
        void zoom(float dr);
        void pan(float du, float dv);
        void rotate(float du, float dv);
        void setPerspective();

        auto viewMatrix() -> mat4 &;
        auto projectionMatrix() -> mat4 &;

    private:
        float r_ = 1.0;
        float theta_ = kPi / 2.0;
        float phi_ = kPi / 2.0;

        vec3 displacement_ = vec3(0);
        vec3 eye_ = vec3(0, 0, 1);
        vec3 center_ = vec3(0);
        vec3 up_ = vec3(0, 1, 0);

        mat4 view_matrix_ = mat4(1.0f);
        mat4 projection_matrix_ = mat4(1.0f);

        void compile();

        auto leftDirection() -> vec3;
        auto downDirection() -> vec3;
    };

}// namespace vx::ctrl
