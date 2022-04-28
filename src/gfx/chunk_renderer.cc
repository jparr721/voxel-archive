#include "chunk_renderer.h"
#include "primitive.h"
#include <iostream>
#include <spdlog/spdlog.h>

namespace vx::gfx {
    void pindices(BlockDir::BlockDirIndices indices) {
        for (const auto &index : indices) { std::cout << index << " "; }
        std::cout << std::endl;
    }

    void pblock(std::vector<VertexColorHex> block) {
        for (const auto &[pos, _] : block) { std::cout << glm::to_string(ivec3(pos)) << " "; }
        std::cout << std::endl;
    }

    ChunkRenderer::ChunkRenderer(const std::vector<Chunk> &chunks) : chunks_(chunks) {
        // TODO Support other vertex types
        vertexLayout_.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();

        for (const auto &chunk : chunks_) {
            const auto geometrySize = sizeof(gfx::VertexColorHex) * chunk.geometry.size();
            const auto indicesSize = sizeof(u16) * chunk.indices.size();
            const auto vb =
                    bgfx::createDynamicVertexBuffer(bgfx::makeRef(chunk.geometry.data(), geometrySize), vertexLayout_);
            const auto ib = bgfx::createDynamicIndexBuffer(bgfx::makeRef(chunk.indices.data(), indicesSize));
            buffers_.emplace_back(vb, ib);
        }
    }

    void ChunkRenderer::render(const bgfx::ProgramHandle &program) {
        // TODO Need to add a program loader here since each chunk will be its own fs vs combo.
        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

        for (const auto &[vertexBuffer, indexBuffer] : buffers_) {
            bgfx::setVertexBuffer(0, vertexBuffer);
            bgfx::setIndexBuffer(indexBuffer);

            bgfx::setState(state);
            bgfx::submit(0, program);
        }
    }

    void ChunkRenderer::destroy() {
        for (const auto &[vb, ib] : buffers_) {
            bgfx::destroy(vb);
            bgfx::destroy(ib);
        }
    }
}// namespace vx::gfx