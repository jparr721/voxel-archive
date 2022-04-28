#pragma once

#include "../math.h"
#include "bgfx.h"
#include "chunk.h"
#include <array>
#include <utility>
#include <vector>

namespace vx::gfx {
    class ChunkRenderer {
    public:
        ChunkRenderer(const std::vector<Chunk> &chunks);
        void render(const bgfx::ProgramHandle &program);
        void destroy();

        auto vertexLayout() const -> const bgfx::VertexLayout & { return vertexLayout_; }

    private:
        const std::vector<Chunk> chunks_;
        bgfx::VertexLayout vertexLayout_;
        std::vector<std::pair<bgfx::DynamicVertexBufferHandle, bgfx::DynamicIndexBufferHandle>> buffers_;
    };
}// namespace vx::gfx