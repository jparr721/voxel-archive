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

        std::vector<BlockIndexSize> indices;
        std::vector<VertexColorHex> geometry;

        explicit Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation = vec3(0, 0, 0),
                       std::string moduleName = "core", std::string _identifier = "Chunk", bool _isFixture = false,
                       const BlockType &_blockType = BlockType::kDebug);
        Chunk(bool _isFixture, BlockType _blockType, std::string _identifier, std::string _shaderModule, int _xdim,
              int _ydim, int _zdim, int _xtransform, int _ytransform, int _ztransform,
              std::vector<BlockIndexSize> _indices, std::vector<VertexColorHex> _geometry)
            : isFixture(_isFixture), blockType(_blockType), identifier(std::move(_identifier)),
              shaderModule(std::move(_shaderModule)), xdim(_xdim), ydim(_ydim), zdim(_zdim), xtransform(_xtransform),
              ytransform(_ytransform), ztransform(_ztransform), indices(std::move(_indices)),
              geometry(std::move(_geometry)) {}

        void write() const noexcept;

        auto operator=(const gfx::Chunk &chunk) -> Chunk & = default;
        auto operator==(const gfx::Chunk &other) const -> bool;

        static auto load(const std::filesystem::path &path) -> std::optional<Chunk>;
    };
}// namespace vx::gfx
