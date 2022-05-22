#include "chunk_renderer.h"
#include "primitive.h"
// temporary
#include "../level_editor/project.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <utility>

namespace vx::gfx {
    ChunkRenderer::ChunkRenderer() {
        vertexLayout_.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                // Setting attrib to float instead of uint8 allows for vec4 colors
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
                .end();
    }

    void ChunkRenderer::addChunk(const Chunk &chunk) {
        // Initialize with the size of the chunk memory
        const auto geometrySize = sizeof(chunk.geometry[0]) * chunk.geometry.size();
        const auto indicesSize = sizeof(chunk.indices[0]) * chunk.indices.size();
        const auto vb = bgfx::createDynamicVertexBuffer(geometrySize, vertexLayout_);
        const auto ib = bgfx::createDynamicIndexBuffer(indicesSize, BGFX_BUFFER_INDEX32);

        // Insert into the stack of buffers. This keeps the memory alive for usage. We utilize
        // this down the chain when we want to swap buffers into the frame
        buffers_.insert({chunk.id, {vb, ib}});
    }

    void ChunkRenderer::removeChunk(const uuids::uuid &chunkIdentifier) {
        const auto &[vertexBuffer, indexBuffer] = buffers_.at(chunkIdentifier);
        bgfx::destroy(vertexBuffer);
        bgfx::destroy(indexBuffer);
        buffers_.erase(chunkIdentifier);
    }

    void ChunkRenderer::render(const bgfx::ProgramHandle &program) {
        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

        for (const auto &[_id, bufferPair] : buffers_) {
            const auto &[vertexBuffer, indexBuffer] = bufferPair;

            // This fucking sucks
            auto &chunk = level_editor::Project::instance()->getChunkByIdentifier(_id);

            if (chunk.needsUpdate) {
                bgfx::update(vertexBuffer, 0,
                             bgfx::copy(&chunk.geometry[0], chunk.geometry.size() * sizeof(chunk.geometry[0])));
                bgfx::update(indexBuffer, 0,
                             bgfx::copy(&chunk.indices[0], chunk.indices.size() * sizeof(chunk.indices[0])));
                chunk.needsUpdate = false;
            }


            bgfx::setVertexBuffer(0, vertexBuffer);
            bgfx::setIndexBuffer(indexBuffer);

            bgfx::setState(state);
            bgfx::submit(0, program);
        }
    }

    void ChunkRenderer::destroy() {
        for (const auto &[_id, bufferPair] : buffers_) {
            const auto &[vertexBuffer, indexBuffer] = bufferPair;
            bgfx::destroy(vertexBuffer);
            bgfx::destroy(indexBuffer);
        }
    }
}// namespace vx::gfx
