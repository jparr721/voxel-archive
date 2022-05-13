#include "./project.h"
#include "../paths.h"
#include "../util/strings.h"

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
    void Project::deleteChunk(const gfx::Chunk &chunk) {
        chunkStorage_->deleteChunk(chunk);

        // Delete the file
        // So, right now this is _pretty_ dumb. The project should just tell the runtime where everything else
        // goes. For now, we do it this way, so, TODO - Fix this
        const auto objectPath = chunk.isFixture ? paths::kFixturesPath : paths::kGameObjetsPath;
        const std::string filenameDotChunk = chunk.identifier + ".chunk";
        for (const auto &iterVal : fs::directory_iterator(objectPath)) {
            if (iterVal.is_regular_file()) {
                if (iterVal.path().filename().extension() == ".chunk" &&
                    util::stringEndsWith(iterVal.path().string(), filenameDotChunk)) {
                    const fs::path deletedFilePath = objectPath / filenameDotChunk;

                    // Delete the file
                    if (!fs::remove(deletedFilePath)) {
                        spdlog::error("File could not be deleted!");
                        break;
                    } else {
                        spdlog::info("File deleted successfully");
                        break;
                    }
                }
            }
        }
    }

    auto Project::getChunks() const -> const std::vector<gfx::Chunk> & { return chunkStorage_->chunks(); }
    auto Project::getChunkByIdentifier(const std::string &identifier) const -> std::optional<const gfx::Chunk> {
        for (const auto &chunk : chunkStorage_->chunks()) {
            if (chunk.identifier == identifier) { return chunk; }
        }

        return std::nullopt;
    }

    void Project::write() {}
}// namespace vx::level_editor
