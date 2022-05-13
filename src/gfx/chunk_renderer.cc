#include "chunk_renderer.h"
#include "primitive.h"
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
        const auto geometrySize = sizeof(gfx::VertexColorHex) * chunk.geometry.size();
        const auto indicesSize = sizeof(u16) * chunk.indices.size();
        const auto vb =
                bgfx::createDynamicVertexBuffer(bgfx::makeRef(chunk.geometry.data(), geometrySize), vertexLayout_);
        const auto ib = bgfx::createDynamicIndexBuffer(bgfx::makeRef(chunk.indices.data(), indicesSize));
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
