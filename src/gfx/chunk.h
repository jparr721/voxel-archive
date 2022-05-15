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
        bool isFixture = false;

        BlockType blockType = BlockType::kDebug;

        std::string identifier;
        std::string shaderModule;

        float xmin = std::numeric_limits<float>::max();
        float xmax = std::numeric_limits<float>::min();
        float ymin = std::numeric_limits<float>::max();
        float ymax = std::numeric_limits<float>::min();
        float zmin = std::numeric_limits<float>::max();
        float zmax = std::numeric_limits<float>::min();

        int xdim;
        int ydim;
        int zdim;

        std::vector<u16> indices;
        std::vector<VertexColorHex> geometry;

        explicit Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation = vec3(0, 0, 0),
                       std::string moduleName = "core", std::string _identifier = "Chunk", bool _isFixture = false,
                       const BlockType &_blockType = BlockType::kDebug);
        void write() const noexcept;

        auto operator=(const gfx::Chunk &chunk) -> Chunk & = default;
        auto operator==(const gfx::Chunk &other) const -> bool;

    private:
        void setBounds();
    };
}// namespace vx::gfx
