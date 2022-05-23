#pragma once

#include "bgfx.h"
#include "chunk.h"
#include "chunk_renderer.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace vx::gfx {
    class ChunkStorage {
    public:
        void render();
        void destroy();
        void addChunk(const Chunk &chunk, bool write = true);
        void deleteChunk(const uuids::uuid &chunkIdentifier);

        auto chunks() -> std::unordered_map<uuids::uuid, Chunk> & { return chunks_; }
        auto chunks() const -> const std::unordered_map<uuids::uuid, Chunk> & { return chunks_; }

    private:
        std::unordered_map<std::string, std::unique_ptr<ChunkRenderer>> renderers_;
        std::unordered_map<uuids::uuid, Chunk> chunks_;
        std::unordered_map<std::string, bgfx::ProgramHandle> shaderPrograms_;
    };
}// namespace vx::gfx
