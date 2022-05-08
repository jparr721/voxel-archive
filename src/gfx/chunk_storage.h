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
        ChunkStorage(const ChunkStorage &cs) = delete;
        auto operator=(const ChunkStorage &cs) -> ChunkStorage & = delete;
        static auto getInstance() -> ChunkStorage *;

        void render();
        void destroy();
        void addChunk(const Chunk &chunk);

    private:
        std::unordered_map<std::string, std::unique_ptr<ChunkRenderer>> renderers_;
        std::vector<Chunk> chunks_;
        std::unordered_map<std::string, bgfx::ProgramHandle> shaderPrograms_;

        static ChunkStorage *chunkStorage_;
        ChunkStorage() = default;

        void loadChunks();
    };
}// namespace vx::gfx