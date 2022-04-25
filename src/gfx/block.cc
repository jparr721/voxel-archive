#include "block.h"
#include <spdlog/spdlog.h>

namespace vx::gfx {
    void applyBlockDirOffset(u64 amount, BlockDir::BlockDirIndices &blockDirIndices) {
        for (auto &index : blockDirIndices) { index += kCubeVertices.size() * amount; }
    }

    Block::Block(BlockType _blockType, BlockDir::BlockDirIndices _blockDirIndices)
        : blockType(_blockType), blockDirIndices(_blockDirIndices) {
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
    }

    void translateBlock(const vec3 &diff, Block &block) {
        for (auto &[pos, _] : block.blockVertexColors) { pos += diff; }
    }
}// namespace vx::gfx