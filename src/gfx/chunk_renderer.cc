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

    ChunkRenderer::ChunkRenderer(const Chunk &chunk) : chunk_(chunk) {
        // TODO Support other vertex types
        vertexLayout_.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .end();


        for (int ii = 0; ii < chunk.blocks.size(); ++ii) {
            bufferData_.emplace_back(chunk.blocks.at(ii).blockVertexColors, chunk.blocks.at(ii).blockDirIndices);
            const auto geometrySize = sizeof(gfx::VertexColorHex) * bufferData_.at(ii).first.size();
            const auto indicesSize = sizeof(u16) * bufferData_.at(ii).second.size();
            const auto vb = bgfx::createDynamicVertexBuffer(
                    bgfx::makeRef(bufferData_.at(ii).first.data(), geometrySize), vertexLayout_);
            const auto ib =
                    bgfx::createDynamicIndexBuffer(bgfx::makeRef(bufferData_.at(ii).second.data(), indicesSize));
            buffers_.emplace_back(vb, ib);
        }
    }

    void ChunkRenderer::render() {
        // TODO Need to add a program loader here since each chunk will be its own fs vs combo.
        u64 state = BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_DEPTH_TEST_LESS;

        bgfx::setVertexBuffer(0, buffers_.front().first);
        bgfx::setIndexBuffer(buffers_.front().second);

        bgfx::setState(state);
    }

    void ChunkRenderer::destroy() {
        // bgfx::destroy(vertexBuffer_);
        // bgfx::destroy(indexBuffer_);

        for (const auto &[vb, ib] : buffers_) {
            bgfx::destroy(vb);
            bgfx::destroy(ib);
        }
    }
}// namespace vx::gfx