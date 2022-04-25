#include "block.h"
#include <spdlog/spdlog.h>

namespace vx::gfx {
    Block::Block(BlockType _blockType, BlockDir::BlockDirIndices _blockDirIndices)
        : blockType(_blockType), blockDirIndices(_blockDirIndices) {
        // Initialize the layout
        vertexLayout.begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8)
                .end();

        u32 color;
        switch (blockType) {
            case BlockType::kDefault:
                color = 0xffffffff;
                break;
            case BlockType::kDirt:
                color = 0xff000000;
                break;
            case BlockType::kGrass:
                color = 0xffff0000;
                break;
        }

        for (const auto &vertex : kCubeVertices) { blockVertexColors.emplace_back(vertex, color); }

        vertexBuffer = bgfx::createDynamicVertexBuffer(
                bgfx::makeRef(blockVertexColors.data(), gfx::VertexColorHex::size() * sizeof(blockVertexColors.data())),
                vertexLayout);
        indexBuffer = bgfx::createDynamicIndexBuffer(bgfx::makeRef(blockDirIndices.data(), sizeof(blockDirIndices)));
    }

    void translateBlock(const vec3 &diff, Block *block) {
        for (auto &[pos, _] : block->blockVertexColors) { pos += diff; }
    }
}// namespace vx::gfx