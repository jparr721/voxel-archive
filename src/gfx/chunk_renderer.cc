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
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();
    }

    void ChunkRenderer::addChunk(const Chunk &chunk) {
        const auto geometrySize = sizeof(chunk.geometry[0]) * chunk.geometry.size();
        const auto indicesSize = sizeof(chunk.indices[0]) * chunk.indices.size();
        /* const auto indicesSize = 16 * chunk.indices.size(); */
        const auto vb = bgfx::createDynamicVertexBuffer(geometrySize, vertexLayout_);
        const auto ib = bgfx::createDynamicIndexBuffer(indicesSize, BGFX_BUFFER_INDEX32);
        buffers_.emplace_back(chunk.identifier, vb, ib);
    }

    void ChunkRenderer::removeChunk(const Chunk &chunk) {
        for (int ii = 0; ii < buffers_.size(); ++ii) {
            const auto &[identifier, vertexBuffer, indexBuffer] = buffers_.at(ii);
            if (chunk.identifier == identifier) {
                bgfx::destroy(vertexBuffer);
                bgfx::destroy(indexBuffer);
                buffers_.erase(buffers_.begin() + ii);
            }
        }
    }

    void ChunkRenderer::render(const bgfx::ProgramHandle &program) {
        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

        for (const auto &[_id, vertexBuffer, indexBuffer] : buffers_) {
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
        for (const auto &[_id, vertexBuffer, indexBuffer] : buffers_) {
            bgfx::destroy(vertexBuffer);
            bgfx::destroy(indexBuffer);
        }
    }
}// namespace vx::gfx
