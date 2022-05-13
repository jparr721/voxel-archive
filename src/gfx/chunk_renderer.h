#pragma once

#include "../math.h"
#include "bgfx.h"
#include "chunk.h"
#include <array>
#include <unordered_map>
#include <utility>
#include <vector>

namespace vx::gfx {
    class ChunkRenderer {
    public:
        ChunkRenderer();

        void addChunk(const Chunk &chunk);

        /**
         * Remove a chunk
         * @param {Chunk} chunk - The chunk we're removing
         */
        void removeChunk(const Chunk &chunk);

        void render(const bgfx::ProgramHandle &program);
        void destroy();

        auto vertexLayout() const -> const bgfx::VertexLayout & { return vertexLayout_; }

    private:
        struct IdentifiedBuffer {
            std::string identifier;
            bgfx::DynamicVertexBufferHandle vertexBuffer;
            bgfx::DynamicIndexBufferHandle indexBuffer;
            IdentifiedBuffer(std::string _identifier, bgfx::DynamicVertexBufferHandle _vertexBuffer,
                             bgfx::DynamicIndexBufferHandle _indexBuffer)
                : identifier(_identifier), vertexBuffer(_vertexBuffer), indexBuffer(_indexBuffer) {}
        };

        bgfx::VertexLayout vertexLayout_;
        std::vector<IdentifiedBuffer> buffers_;
    };
}// namespace vx::gfx
