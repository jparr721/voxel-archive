#include "chunk_renderer.h"
#include "primitive.h"
#include <spdlog/spdlog.h>

namespace vx::gfx {
    ChunkRenderer::ChunkRenderer(const Chunk &chunk) : chunk_(chunk) {
        const auto &[geometry, indices] = chunk.stacked();
        geometry_ = geometry;
        indices_ = indices;

        // TODO Support other vertex types
        vertexLayout_.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
                .end();

        const auto geometrySize = gfx::VertexColorHex::size() * geometry_.size();
        const auto indicesSize = sizeof(u16) * indices_.size();
        vertexBuffer_ = bgfx::createDynamicVertexBuffer(bgfx::makeRef(geometry_.data(), geometrySize), vertexLayout_);
        indexBuffer_ = bgfx::createDynamicIndexBuffer(bgfx::makeRef(indices_.data(), indicesSize));
    }

    void ChunkRenderer::render() {
        // TODO Need to add a program loader here since each chunk will be its own fs vs combo.
        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

        bgfx::setVertexBuffer(0, vertexBuffer_);
        bgfx::setIndexBuffer(indexBuffer_);

        bgfx::setState(state);
    }

    void ChunkRenderer::destroy() {
        bgfx::destroy(vertexBuffer_);
        bgfx::destroy(indexBuffer_);
    }
}// namespace vx::gfx