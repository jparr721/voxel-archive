#pragma once

#include "../math.h"

namespace vx::gfx {
    struct VertexColor {
        vec3 position;
        vec4 color;
        VertexColor() = default;
        VertexColor(const vec3 &position, const vec4 &color) : position(position), color(color) {}
    };

    struct VertexColorHex {
        vec3 position;
        u32 color;
        VertexColorHex() = default;
        VertexColorHex(const vec3 &position, const u32 &color) : position(position), color(color) {}
    };

    struct VertexTexture {
        vec3 position;
        vec2 textureCoordinates;
        VertexTexture() = default;
        VertexTexture(const vec3 &position, const vec2 &textureCoordinates)
            : position(position), textureCoordinates(textureCoordinates) {}
    };

    struct VertexTextureColor {
        vec3 position;
        vec4 color;
        vec2 textureCoordinates;
        VertexTextureColor() = default;
        VertexTextureColor(const vec3 &position, const vec4 &color, const vec2 &textureCoordinates)
            : position(position), color(color), textureCoordinates(textureCoordinates){};
    };
}// namespace vx::gfx
