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
                break;
            case BlockType::kGrass:
                break;
        }

        for (const auto &vertex : kCubeVertices) { blockVertexColors.emplace_back(vertex, color); }

        vertexBuffer = bgfx::createVertexBuffer(
                bgfx::makeRef(blockVertexColors.data(), gfx::VertexColorHex::size() * sizeof(blockVertexColors.data())),
                vertexLayout);
        indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(blockDirIndices.data(), sizeof(blockDirIndices)));
    }
}// namespace vx::gfx