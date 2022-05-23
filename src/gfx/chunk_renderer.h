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
         * Delete a chunk
         * @param {uuids::uuid} chunkIdentifier - The ID of the chunk we're removing
         */
        void deleteChunk(const uuids::uuid &chunkIdentifier);

        void render(const bgfx::ProgramHandle &program);
        void destroy();

        auto vertexLayout() const -> const bgfx::VertexLayout & { return vertexLayout_; }

    private:
        struct BufferPair {
            bgfx::DynamicVertexBufferHandle vertexBuffer;
            bgfx::DynamicIndexBufferHandle indexBuffer;
        };

        bgfx::VertexLayout vertexLayout_;
        std::unordered_map<uuids::uuid, BufferPair> buffers_;
    };
}// namespace vx::gfx
