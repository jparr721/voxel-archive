#include "chunk_storage.h"
#include "../paths.h"
#include "../resources.h"

namespace vx::gfx {
    void ChunkStorage::render() {
        // TODO (@jparr721) Make this less dumb.
        for (const auto &[_, chunk] : chunks_) {
            const auto moduleName = chunk.shaderModule;
            renderers_.at(moduleName)->render(shaderPrograms_.at(moduleName));
        }
    }

    void ChunkStorage::destroy() {
        spdlog::info("Destroying Chunk Storage");
        for (const auto &[_, renderer] : renderers_) { renderer->destroy(); }
        for (const auto &[_, program] : shaderPrograms_) { bgfx::destroy(program); }
    }

    void ChunkStorage::addChunk(const Chunk &chunk, bool write) {
        chunks_.insert({chunk.id, chunk});
        if (write) { chunk.write(); }

        // Add the chunk to the shader programs if it's not already there
        if (shaderPrograms_.find(chunk.shaderModule) == shaderPrograms_.end()) {
            shaderPrograms_.insert(
                    {chunk.shaderModule, vx::loadShaderProgram(paths::kShadersPath, chunk.shaderModule)});
            renderers_.insert({chunk.shaderModule, std::make_unique<ChunkRenderer>()});
        }

        // Now, add the chunk to the appropriate renderer
        renderers_.at(chunk.shaderModule)->addChunk(chunk);
    }

    void ChunkStorage::deleteChunk(const uuids::uuid &chunkIdentifier) {
        const auto &chunk = chunks_.at(chunkIdentifier);
        renderers_.at(chunk.shaderModule)->removeChunk(chunk.id);
        chunks_.erase(chunkIdentifier);
    }

    void ChunkStorage::setChunk(const Chunk &newChunk) {
        // No op this until we can debug it
        /* for (auto &chunk : chunks_) { */
        /*     if (chunk.identifier == newChunk.identifier) { */
        /* #ifndef NDEBUG */
        /*         const auto chunksSize = chunks_.size(); */
        /* #endif */
        /*         chunk = newChunk; */
        /* #ifndef NDEBUG */
        /*         assert(chunksSize == chunks_.size()); */
        /* #endif */
        /*         break; */
        /*     } */
        /* } */
    }
}// namespace vx::gfx
