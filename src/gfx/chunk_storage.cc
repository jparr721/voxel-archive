#include "chunk_storage.h"
#include "../resources.h"

namespace vx::gfx {
    ChunkStorage *ChunkStorage::chunkStorage_ = nullptr;

    auto ChunkStorage::getInstance() -> ChunkStorage * {
        if (chunkStorage_ == nullptr) { chunkStorage_ = new ChunkStorage(); }
        return chunkStorage_;
    }

    void ChunkStorage::render() {
        for (const auto &chunk : chunks_) {
            const auto moduleName = chunk.shaderModule;
            renderers_.at(moduleName)->render(shaderPrograms_.at(moduleName));
        }
    }

    void ChunkStorage::destroy() {
        spdlog::info("Destroying Chunk Storage");
        for (const auto &[_, renderer] : renderers_) { renderer->destroy(); }
        for (const auto &[_, program] : shaderPrograms_) { bgfx::destroy(program); }
    }

    void ChunkStorage::addChunk(const Chunk &chunk) {
        chunks_.push_back(chunk);
        loadChunks();
    }

    void ChunkStorage::loadChunks() {
#if BX_PLATFORM_WINDOWS
        const std::string basePath = "../resources/shaders";
#else
        const std::string basePath = "resources/shaders";
#endif
        for (const auto &chunk : chunks_) {
            const auto moduleName = chunk.shaderModule;
            // Load the shader program
            if (shaderPrograms_.find(moduleName) == shaderPrograms_.end()) {
                const auto shaderProgram = vx::loadShaderProgram(basePath, moduleName);
                shaderPrograms_.insert({moduleName, shaderProgram});
            }

            // Load the chunk renderer
            if (renderers_.find(moduleName) == renderers_.end()) {
                renderers_.insert({moduleName, std::make_unique<ChunkRenderer>()});
            }
        }

        for (const auto &chunk : chunks_) {
            const auto moduleName = chunk.shaderModule;
            renderers_.at(moduleName)->addChunk(chunk);
        }
    }
}// namespace vx::gfx
