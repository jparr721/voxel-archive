#pragma once

#include "../gfx/chunk.h"
#include "../gfx/chunk_storage.h"
#include <filesystem>
#include <memory>
#include <optional>
#include <pugixml.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

namespace vx::level_editor {
    class Project {
    public:
        std::string name = "Level";

        Project(const Project &cs) = delete;
        auto operator=(const Project &cs) -> Project & = delete;

        static auto instance() -> Project *;

        void render();
        void destroy();
        void addChunk(const gfx::Chunk &chunk);
        void deleteChunk(const uuids::uuid &chunkIdentifier);

        // ====== Getters
        //
        auto storage() -> std::unique_ptr<gfx::ChunkStorage> & { return chunkStorage_; }
        auto getChunks() -> std::unordered_map<uuids::uuid, gfx::Chunk> &;
        auto getChunkByIdentifier(const uuids::uuid &chunkIdentifier) -> gfx::Chunk &;

        // Project file management
        auto projectVersionString() -> std::string;
        auto projectXMLHeader(pugi::xml_document &doc) -> pugi::xml_node;

        // Project path management
        auto projectFilePath() -> fs::path;
        auto projectFolderPath() -> fs::path;
        auto fixtureFolderPath() -> fs::path;
        auto gameObjectFolderPath() -> fs::path;

    private:
        Project();

        // Unique ref to chunk storage
        std::unique_ptr<gfx::ChunkStorage> chunkStorage_;

        /**
         * Write to the project configuration.
         */
        void write();
        void load();
    };
}// namespace vx::level_editor
