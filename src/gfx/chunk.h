#pragma once

#include "bgfx.h"
#include "block.h"
#include "primitive.h"
#include <string>
#include <utility>
#include <vector>

namespace vx::gfx {
    class Chunk {
    public:
        std::string identifier;
        std::string shaderModule;

        float minX;
        float maxX;

        float minY;
        float maxY;

        float minZ;
        float maxZ;

        std::vector<u16> indices;
        std::vector<VertexColorHex> geometry;

        explicit Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation = vec3(0, 0, 0),
                       std::string moduleName = "core", std::string identifier = "Chunk");
        void write(bool isFixture) noexcept;
    private:
        void setBounds();
    };

    void translateChunk(const vec3 &amount, Chunk &chunk);
}// namespace vx::gfx