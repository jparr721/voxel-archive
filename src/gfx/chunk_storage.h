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
        void addChunk(const Chunk &chunk);
        void deleteChunk(const std::string &chunkIdentifier);
        void setChunk(const Chunk &newChunk);

        auto chunks() -> std::vector<Chunk> & { return chunks_; }
        auto chunks() const -> const std::vector<Chunk> & { return chunks_; }

        void loadChunks();

    private:
        std::unordered_map<std::string, std::unique_ptr<ChunkRenderer>> renderers_;
        std::vector<Chunk> chunks_;
        std::unordered_map<std::string, bgfx::ProgramHandle> shaderPrograms_;
    };
}// namespace vx::gfx
