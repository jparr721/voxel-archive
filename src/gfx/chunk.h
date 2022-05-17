#pragma once

#include "bgfx.h"
#include "block.h"
#include "primitive.h"
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace vx::gfx {
    struct Chunk {
        bool isFixture = false;

        BlockType blockType = BlockType::kDebug;

        std::string identifier;
        std::string shaderModule;

        int xdim;
        int ydim;
        int zdim;

        int xtransform;
        int ytransform;
        int ztransform;

        std::vector<u16> indices;
        std::vector<VertexColorHex> geometry;

        explicit Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation = vec3(0, 0, 0),
                       std::string moduleName = "core", std::string _identifier = "Chunk", bool _isFixture = false,
                       const BlockType &_blockType = BlockType::kDebug);
        void write() const noexcept;

        auto operator=(const gfx::Chunk &chunk) -> Chunk & = default;
        auto operator==(const gfx::Chunk &other) const -> bool;

        static auto load(const std::filesystem::path &path) -> std::optional<Chunk>;
    };
}// namespace vx::gfx
