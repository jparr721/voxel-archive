#pragma once

#include "../math.h"
#include "bgfx.h"
#include "chunk.h"
#include <array>

namespace vx::gfx {
    class ChunkRenderer {
    public:
        ChunkRenderer(const Chunk &chunk);
        void render();
        void destroy();

        auto vertexLayout() const -> const bgfx::VertexLayout & { return vertexLayout_; }
        auto vertexBuffer() const -> const bgfx::DynamicVertexBufferHandle & { return vertexBuffer_; }
        auto indexBuffer() const -> const bgfx::DynamicIndexBufferHandle & { return indexBuffer_; }

    private:
        const Chunk chunk_;

        std::vector<VertexColorHex> geometry_;
        std::vector<u16> indices_;

        bgfx::VertexLayout vertexLayout_;
        bgfx::DynamicVertexBufferHandle vertexBuffer_;
        bgfx::DynamicIndexBufferHandle indexBuffer_;
    };
}// namespace vx::gfx