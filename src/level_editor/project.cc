#include "./project.h"

namespace vx::level_editor {
    Project *Project::project_ = nullptr;
    auto Project::getInstance() -> Project * {
        if (project_ == nullptr) { project_ = new Project(); }
        return project_;
    }

    Project::Project() { chunkStorage_ = std::make_unique<gfx::ChunkStorage>(); }

    void Project::render() { chunkStorage_->render(); }
    void Project::destroy() { chunkStorage_->destroy(); }
    void Project::addChunk(const gfx::Chunk &chunk) { chunkStorage_->addChunk(chunk); }
    void Project::setChunk(const std::string &identifier, const gfx::Chunk &chunk) { chunkStorage_->setChunk(chunk); }
    void Project::deleteChunk(const gfx::Chunk &chunk) { chunkStorage_->deleteChunk(chunk); }

    auto Project::getChunks() const -> const std::vector<gfx::Chunk> & { return chunkStorage_->chunks(); }
    auto Project::getChunkByIdentifier(const std::string &identifier) const -> std::optional<const gfx::Chunk> {
        for (const auto &chunk : chunkStorage_->chunks()) {
            if (chunk.identifier == identifier) { return chunk; }
        }

        return std::nullopt;
    }

    void Project::write() {}
}// namespace vx::level_editor
