#include "./project.h"
#include "../paths.h"
#include "../util/strings.h"
#include <fstream>
#include <iostream>
#include <pugixml.hpp>

constexpr int kProjectFileVersionMajor = 0;
constexpr int kProjectFileVersionMinor = 1;
constexpr int kProjectFileVersionPatch = 0;

namespace vx::level_editor {
    Project *Project::project_ = nullptr;
    auto Project::getInstance() -> Project * {
        if (project_ == nullptr) { project_ = new Project(); }
        return project_;
    }

    Project::Project() {
        chunkStorage_ = std::make_unique<gfx::ChunkStorage>();

        // Make projects folder
        if (!fs::exists(projectFolderPath())) {
            fs::create_directory(projectFolderPath());
        } else {
            /* Load */
        }

        // Make fixtures
        if (!fs::exists(fixtureFolderPath())) { fs::create_directory(fixtureFolderPath()); }

        // Make game objects
        if (!fs::exists(gameObjectFolderPath())) { fs::create_directory(gameObjectFolderPath()); }
    }

    void Project::render() { chunkStorage_->render(); }
    void Project::destroy() { chunkStorage_->destroy(); }
    void Project::addChunk(const gfx::Chunk &chunk) {
        chunkStorage_->addChunk(chunk);
        if (chunk.isFixture) {
            fixtureChunks_.push_back(chunk);
        } else {
            gameObjectChunks_.push_back(chunk);
        }
        write();
    }

    void Project::setChunk(const gfx::Chunk &chunk) {
        chunkStorage_->setChunk(chunk);
        write();
    }

    void Project::deleteChunk(const gfx::Chunk &chunk) {
        chunkStorage_->deleteChunk(chunk);

        // Delete the file
        // So, right now this is _pretty_ dumb. The project should just tell the runtime where everything else
        // goes. For now, we do it this way, so, TODO - Fix this
        const auto objectPath = chunk.isFixture ? fixtureFolderPath() : gameObjectFolderPath();
        const std::string filenameWithExtension = chunk.identifier + paths::kXmlPostfix;
        for (const auto &iterVal : fs::directory_iterator(objectPath)) {
            if (iterVal.is_regular_file()) {
                if (iterVal.path().filename().extension() == paths::kXmlPostfix &&
                    util::stringEndsWith(iterVal.path().string(), filenameWithExtension)) {
                    const fs::path deletedFilePath = objectPath / filenameWithExtension;

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

        // Delete the value from the object path
        if (chunk.isFixture) {
            for (int ii = 0; ii < fixtureChunks_.size(); ++ii) {
                const auto &fixtureChunk = fixtureChunks_.at(ii);
                if (fixtureChunk.identifier == chunk.identifier) {
                    fixtureChunks_.erase(fixtureChunks_.begin() + ii);
                    break;
                }
            }
        } else {
            for (int ii = 0; ii < gameObjectChunks_.size(); ++ii) {
                const auto &gameObjectChunk = gameObjectChunks_.at(ii);
                if (gameObjectChunk.identifier == chunk.identifier) {
                    gameObjectChunks_.erase(gameObjectChunks_.begin() + ii);
                    break;
                }
            }
        }

        // Write the current state
        write();
    }

    auto Project::getChunks() const -> const std::vector<gfx::Chunk> & { return chunkStorage_->chunks(); }
    auto Project::getChunkByIdentifier(const std::string &identifier) const -> std::optional<const gfx::Chunk> {
        for (const auto &chunk : chunkStorage_->chunks()) {
            if (chunk.identifier == identifier) { return chunk; }
        }

        return std::nullopt;
    }

    void Project::write() {
        pugi::xml_document projectDocument;
        pugi::xml_node projectNode = projectDocument.append_child("project");
        projectNode.append_attribute("name") = name.c_str();
        projectNode.append_attribute("version") =
                util::semverToString(kProjectFileVersionMajor, kProjectFileVersionMinor, kProjectFileVersionPatch)
                        .c_str();

        // Paths for the fixtures associated with this project.
        pugi::xml_node fixturesNode = projectNode.append_child("fixtures");
        fixturesNode.append_attribute("n_chunks") = fixtureChunks_.size();
        for (const auto &fixtureChunk : fixtureChunks_) {
            pugi::xml_node fixtureNode = fixturesNode.append_child("fixture");
            fixtureNode.append_attribute("name") = fixtureChunk.identifier.c_str();
            fixtureNode.append_attribute("path") =
                    std::string("fixtures/" + fixtureChunk.identifier + paths::kXmlPostfix).c_str();
        }

        // Paths for the game objects
        pugi::xml_node gameObjectsNode = projectNode.append_child("gameObjects");
        gameObjectsNode.append_attribute("n_chunks") = gameObjectChunks_.size();
        for (const auto &gameObjectChunk : gameObjectChunks_) {
            pugi::xml_node gameObjectNode = gameObjectsNode.append_child("gameObject");
            gameObjectNode.append_attribute("name") = gameObjectChunk.identifier.c_str();
            gameObjectNode.append_attribute("path") =
                    std::string("game_objects/" + gameObjectChunk.identifier + paths::kXmlPostfix).c_str();
        }

#ifndef NDEBUG
        projectDocument.print(std::cout);
#endif

        const auto projectPath = paths::kProjectsPath / fs::path(name + paths::kXmlPostfix);
        projectDocument.save_file(projectFilePath().c_str());
    }

    auto Project::projectFilePath() -> fs::path { return projectFolderPath() / "project.xml"; }
    auto Project::projectFolderPath() -> fs::path { return paths::kProjectsPath / name; }
    auto Project::fixtureFolderPath() -> fs::path { return projectFolderPath() / "fixtures"; }
    auto Project::gameObjectFolderPath() -> fs::path { return projectFolderPath() / "game_objects"; }
}// namespace vx::level_editor
