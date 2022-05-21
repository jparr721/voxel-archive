#include "./project.h"
#include "../paths.h"
#include "../util/strings.h"
#include <fstream>
#include <iostream>

constexpr int kProjectFileVersionMajor = 0;
constexpr int kProjectFileVersionMinor = 1;
constexpr int kProjectFileVersionPatch = 0;

namespace vx::level_editor {
    auto Project::instance() -> Project * {
        static std::unique_ptr<Project> project = std::unique_ptr<Project>{new Project()};
        return project.get();
    }

    Project::Project() {
        chunkStorage_ = std::make_unique<gfx::ChunkStorage>();

        // Make project folder
        if (!fs::exists(projectFolderPath())) {
            // Make projects folder
            fs::create_directory(projectFolderPath());
            // Make fixtures
            fs::create_directory(fixtureFolderPath());
            // Make game objects
            fs::create_directory(gameObjectFolderPath());
        } else {// Otherwise, load the existing project.
            load();
        }
    }

    void Project::render() { chunkStorage_->render(); }
    void Project::destroy() { chunkStorage_->destroy(); }
    void Project::addChunk(const gfx::Chunk &chunk) {
        chunkStorage_->addChunk(chunk);
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
        spdlog::info("Attempting to delete file {}", filenameWithExtension);
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

        // Write the current state
        write();
    }

    auto Project::getChunks() -> std::vector<gfx::Chunk> & { return chunkStorage_->chunks(); }
    auto Project::getChunkByIdentifier(const std::string &identifier) -> gfx::Chunk & {
        for (auto &chunk : chunkStorage_->chunks()) {
            if (chunk.identifier == identifier) { return chunk; }
        }

        assert(false && "CHUNK NOT FOUND");
    }

    auto Project::projectVersionString() -> std::string {
        return util::semverToString(kProjectFileVersionMajor, kProjectFileVersionMinor, kProjectFileVersionPatch);
    }
    auto Project::projectXMLHeader(pugi::xml_document &doc) -> pugi::xml_node {
        pugi::xml_node headerNode = doc.append_child("project");
        headerNode.append_attribute("name") = name.c_str();
        headerNode.append_attribute("version") = projectVersionString().c_str();
        return headerNode;
    }

    //
    auto Project::projectFilePath() -> fs::path { return projectFolderPath() / "project.xml"; }
    auto Project::projectFolderPath() -> fs::path { return paths::kProjectsPath / name; }
    auto Project::fixtureFolderPath() -> fs::path { return projectFolderPath() / "fixtures"; }
    auto Project::gameObjectFolderPath() -> fs::path { return projectFolderPath() / "game_objects"; }

    void Project::write() {
        pugi::xml_document projectDocument;
        pugi::xml_node projectNode = projectXMLHeader(projectDocument);

        // Paths for the fixtures associated with this project.
        pugi::xml_node fixturesComponent = projectNode.append_child("component");
        fixturesComponent.append_attribute("name") = "fixtures";

        int nFixtures = 0;
        int nGameObjects = 0;
        for (const auto &chunk : chunkStorage_->chunks()) {
            if (chunk.isFixture) {
                ++nFixtures;
            } else {
                ++nGameObjects;
            }
        }

        pugi::xml_node fixturesComponentNNodesProperty = fixturesComponent.append_child("property");
        fixturesComponentNNodesProperty.append_attribute("name") = "nNodes";
        fixturesComponentNNodesProperty.append_attribute("value") = nFixtures;

        pugi::xml_node fixturesList = fixturesComponent.append_child("fixtures-list");
        for (const auto &fixtureChunk : chunkStorage_->chunks()) {
            if (fixtureChunk.isFixture) {
                pugi::xml_node fixtureNode = fixturesList.append_child("item");
                fixtureNode.append_attribute("name") = fixtureChunk.identifier.c_str();
                fixtureNode.append_attribute("path") =
                        std::string(fixtureChunk.identifier + paths::kXmlPostfix).c_str();
            }
        }

        // Paths for the game objects
        pugi::xml_node gameObjectsComponent = projectNode.append_child("component");
        gameObjectsComponent.append_attribute("name") = "gameObjects";

        pugi::xml_node gameObjectsComponentNNodesProperty = gameObjectsComponent.append_child("property");
        gameObjectsComponentNNodesProperty.append_attribute("name") = "nNodes";
        gameObjectsComponentNNodesProperty.append_attribute("value") = nGameObjects;

        pugi::xml_node gameObjectsList = gameObjectsComponent.append_child("game-objects-list");
        for (const auto &gameObjectChunk : chunkStorage_->chunks()) {
            if (!gameObjectChunk.isFixture) {
                pugi::xml_node gameObjectNode = gameObjectsList.append_child("item");
                gameObjectNode.append_attribute("name") = gameObjectChunk.identifier.c_str();
                gameObjectNode.append_attribute("path") =
                        std::string(gameObjectChunk.identifier + paths::kXmlPostfix).c_str();
            }
        }

#ifndef NDEBUG
        projectDocument.print(std::cout);
#endif

        const auto projectPath = paths::kProjectsPath / fs::path(name + paths::kXmlPostfix);
        projectDocument.save_file(projectFilePath().c_str());
    }

    void Project::load() {
        spdlog::debug("Loading project: {}", projectFolderPath().string());

        // Check to be sure the directory exists
        if (!fs::exists(projectFolderPath())) {
            spdlog::error("Project could not be found at this directory");
            return;
        }

        // Now, first load the project file
        pugi::xml_document projectDocument;

        pugi::xml_parse_result parseResult = projectDocument.load_file(projectFilePath().c_str());

        // If failure, spit out the error
        if (!parseResult) {
#ifndef NDEBUG
            assert(parseResult && "PARSE FAILED FOR PROJECT");
#endif
            spdlog::error("Failed to load project file with error: {}", parseResult.description());
            return;
        }

        // TODO (@jparr721) This assumes we are loading from _empty_, this might break when loading while
        // an existing project is open.
        const pugi::xml_node projectNode = projectDocument.child("project");
        name = projectNode.attribute("name").value();

        // Now, load the fixtures
        const pugi::xml_node fixturesComponent = projectNode.child("component");
        const pugi::xml_node fixturesPropertyNode = fixturesComponent.child("property");
        const pugi::xml_node fixturesList = fixturesPropertyNode.next_sibling();

        for (const auto &child : fixturesList.children()) {
            const auto chunk = gfx::Chunk::load(fixtureFolderPath() / child.attribute("path").value());
            if (!chunk.has_value()) {
                spdlog::error("Chunk {} failed to load", child.attribute("name").value());
            } else {
                chunkStorage_->chunks().push_back(chunk.value());
                chunkStorage_->loadChunks();
            }
        }

        // Now, load the fixtures
        const pugi::xml_node gameObjectsComponent = fixturesComponent.next_sibling();
        const pugi::xml_node gameObjectsPropertyNode = gameObjectsComponent.child("property");
        const pugi::xml_node gameObjectsList = gameObjectsPropertyNode.next_sibling();

        for (const auto &child : gameObjectsList.children()) {
            const auto chunk = gfx::Chunk::load(gameObjectFolderPath() / child.attribute("path").value());
            if (!chunk.has_value()) {
                spdlog::error("Chunk {} failed to load", child.attribute("name").value());
            } else {
                chunkStorage_->chunks().push_back(chunk.value());
                chunkStorage_->loadChunks();
            }
        }
    }
}// namespace vx::level_editor
