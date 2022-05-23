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

    void Project::deleteChunk(const uuids::uuid &chunkIdentifier) {
        // Delete file
        const auto &chunk = getChunkByIdentifier(chunkIdentifier);
        const fs::path filename = chunk.name + paths::kXmlPostfix;

        if (chunk.isFixture) {
            fs::remove(fixtureFolderPath() / filename);
        } else {
            fs::remove(gameObjectFolderPath() / filename);
        }

        // Delete memory
        chunkStorage_->deleteChunk(chunkIdentifier);
        write();
    }

    auto Project::getChunks() -> std::unordered_map<uuids::uuid, gfx::Chunk> & { return chunkStorage_->chunks(); }
    auto Project::getChunkByIdentifier(const uuids::uuid &chunkIdentifier) -> gfx::Chunk & {
        // First, check if the key exists
        if (chunkStorage_->chunks().contains(chunkIdentifier)) { return chunkStorage_->chunks().at(chunkIdentifier); }

        spdlog::error("Chunk not found {}", uuids::to_string(chunkIdentifier));

#ifndef NDEBUG
        spdlog::error("Dumping chunks");
        for (auto &[id, chunk] : chunkStorage_->chunks()) { spdlog::info("chunk {}", chunk.name); }
#endif

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
        for (const auto &[_id, chunk] : chunkStorage_->chunks()) {
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
        for (const auto &[_id, fixtureChunk] : chunkStorage_->chunks()) {
            if (fixtureChunk.isFixture) {
                pugi::xml_node fixtureNode = fixturesList.append_child("item");
                fixtureNode.append_attribute("name") = fixtureChunk.name.c_str();
                fixtureNode.append_attribute("path") = std::string(fixtureChunk.name + paths::kXmlPostfix).c_str();
            }
        }

        // Paths for the game objects
        pugi::xml_node gameObjectsComponent = projectNode.append_child("component");
        gameObjectsComponent.append_attribute("name") = "gameObjects";

        pugi::xml_node gameObjectsComponentNNodesProperty = gameObjectsComponent.append_child("property");
        gameObjectsComponentNNodesProperty.append_attribute("name") = "nNodes";
        gameObjectsComponentNNodesProperty.append_attribute("value") = nGameObjects;

        pugi::xml_node gameObjectsList = gameObjectsComponent.append_child("game-objects-list");
        for (const auto &[_id, gameObjectChunk] : chunkStorage_->chunks()) {
            if (!gameObjectChunk.isFixture) {
                pugi::xml_node gameObjectNode = gameObjectsList.append_child("item");
                gameObjectNode.append_attribute("name") = gameObjectChunk.name.c_str();
                gameObjectNode.append_attribute("path") =
                        std::string(gameObjectChunk.name + paths::kXmlPostfix).c_str();
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

        // doNotWrite tells the runtime to not write these chunks since they already exist.
        constexpr bool kDoNotWrite = false;
        for (const auto &child : fixturesList.children()) {
            const auto chunk = gfx::Chunk::load(fixtureFolderPath() / child.attribute("path").value());
            if (!chunk.has_value()) {
                spdlog::error("Chunk {} failed to load", child.attribute("name").value());
            } else {
                chunkStorage_->addChunk(chunk.value(), kDoNotWrite);
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
                chunkStorage_->addChunk(chunk.value(), kDoNotWrite);
            }
        }
    }
}// namespace vx::level_editor
