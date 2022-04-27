#pragma once

#include "../math.h"
#include "bgfx.h"
#include "chunk.h"
#include <array>
#include <utility>

namespace vx::gfx {
    class ChunkRenderer {
    public:
        ChunkRenderer(const Chunk &chunk);
        void render(const bgfx::ProgramHandle &program);
        void destroy();

        auto vertexLayout() const -> const bgfx::VertexLayout & { return vertexLayout_; }

    private:
        const Chunk chunk_;

        bgfx::VertexLayout vertexLayout_;

        std::vector<std::pair<std::vector<VertexColorHex>, std::vector<u16>>> bufferData_;
        std::vector<std::pair<bgfx::DynamicVertexBufferHandle, bgfx::DynamicIndexBufferHandle>> buffers_;
    };
}// namespace vx::gfx