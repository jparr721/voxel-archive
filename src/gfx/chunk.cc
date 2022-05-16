#include "chunk.h"
#include "../level_editor/project.h"
#include "../paths.h"
#include "../util//strings.h"
#include <fstream>
#include <iostream>
#include <pugixml.hpp>
#include <spdlog/spdlog.h>
#include <utility>

namespace vx::gfx {
    Chunk::Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation, std::string moduleName, std::string _identifier,
                 bool _isFixture, const BlockType &_blockType)
        : shaderModule(std::move(moduleName)), identifier(std::move(_identifier)), isFixture(_isFixture),
          blockType(_blockType) {
        spdlog::debug("Loading chunk id: {} module: {}", identifier, shaderModule);
        xdim = chunkSize.x;
        ydim = chunkSize.y;
        zdim = chunkSize.z;
        // Origin point is always 0 0 0, so we draw from there
        u64 ii = 0;
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    // TODO - Compute block direction
                    BlockDir::BlockDirIndices baseIndices = BlockDir::kDebug;

                    // Increment indices to avoid overlapping faces
                    for (auto &index : baseIndices) {
                        index += kCubeVertices.size() * ii;
                        indices.push_back(index);
                    }

                    const u32 color = makeColorFromBlockType(blockType);
                    const vec3 startingPosition = vec3(xx, yy, zz);
                    for (const auto &vertex : makeOffsetCubeVertices(startingPosition)) {
                        geometry.emplace_back(vertex, color);
                    }
                    ++ii;
                }
            }
        }

        // Translate the chunk
        for (auto &[pos, _] : geometry) { pos += chunkTranslation; }
        spdlog::debug("Chunk loaded successfully");
    }

    void Chunk::write() const noexcept {
        pugi::xml_document chunkDocument;
        pugi::xml_node projectNode = level_editor::Project::getInstance()->projectXMLHeader(chunkDocument);

        pugi::xml_node shaderModuleComponent = projectNode.append_child("component");
        shaderModuleComponent.append_attribute("name") = "shaderModule";
        shaderModuleComponent.append_attribute("value") = shaderModule.c_str();

        pugi::xml_node isFixtureComponent = projectNode.append_child("component");
        isFixtureComponent.append_attribute("name") = "isFixture";
        isFixtureComponent.append_attribute("value") = isFixture;

        pugi::xml_node dimensionsComponent = projectNode.append_child("component");
        dimensionsComponent.append_attribute("name") = "dimensions";
        pugi::xml_node dimensionsMapComponent = dimensionsComponent.append_child("map");
        pugi::xml_node xDimEntry = dimensionsMapComponent.append_child("entry");
        xDimEntry.append_attribute("key") = "xdim";
        xDimEntry.append_attribute("value") = xdim;

        pugi::xml_node yDimEntry = dimensionsMapComponent.append_child("entry");
        yDimEntry.append_attribute("key") = "ydim";
        yDimEntry.append_attribute("value") = ydim;

        pugi::xml_node zDimEntry = dimensionsMapComponent.append_child("entry");
        zDimEntry.append_attribute("key") = "zdim";
        zDimEntry.append_attribute("value") = zdim;

        // Write the indices of this chunk
        pugi::xml_node indicesComponent = projectNode.append_child("component");
        indicesComponent.append_attribute("name") = "indices";

        pugi::xml_node indicesComponentNNodesProperty = indicesComponent.append_child("property");
        indicesComponentNNodesProperty.append_attribute("name") = "nNodes";
        indicesComponentNNodesProperty.append_attribute("value") = indices.size();

        pugi::xml_node indicesList = indicesComponent.append_child("indices-list");
        for (const auto &index : indices) {
            pugi::xml_node itemNode = indicesList.append_child("item");
            itemNode.append_attribute("index") = index;
        }

        // Write the vertices
        pugi::xml_node verticesComponent = projectNode.append_child("component");
        verticesComponent.append_attribute("name") = "vertices";

        pugi::xml_node verticesComponentNNodesProperty = verticesComponent.append_child("property");
        verticesComponentNNodesProperty.append_attribute("name") = "nNodes";
        verticesComponentNNodesProperty.append_attribute("value") = geometry.size();

        pugi::xml_node verticesComponentBlockTypeProperty = verticesComponent.append_child("property");
        verticesComponentBlockTypeProperty.append_attribute("name") = "blockType";
        verticesComponentBlockTypeProperty.append_attribute("value") = gfx::blockTypeToString(blockType).c_str();

        pugi::xml_node verticesComponentVertexTypeProperty = verticesComponent.append_child("property");
        verticesComponentVertexTypeProperty.append_attribute("name") = "vertexType";
        verticesComponentVertexTypeProperty.append_attribute("value") = "VertexColorHex";

        pugi::xml_node verticesList = verticesComponent.append_child("vertices-list");
        int ii = 0;
        for (const auto &vertexColorHex : geometry) {
            const auto &x = vertexColorHex.position.x;
            const auto &y = vertexColorHex.position.y;
            const auto &z = vertexColorHex.position.z;
            const auto &color = vertexColorHex.color;

            // Make a vertex top-level element
            pugi::xml_node vertexNode = verticesList.append_child("item");
            vertexNode.append_attribute("index") = ii;
            vertexNode.append_attribute("xpos") = x;
            vertexNode.append_attribute("ypos") = y;
            vertexNode.append_attribute("zpos") = z;
            ++ii;
        }

#ifndef NDEBUG
        // Debug print
        chunkDocument.print(std::cout);
#endif

        // Save the file to our pre-determined path
        const auto filepath = isFixture ? level_editor::Project::getInstance()->fixtureFolderPath() /
                                                  fs::path(identifier + paths::kXmlPostfix)
                                        : level_editor::Project::getInstance()->gameObjectFolderPath() /
                                                  fs::path(identifier + paths::kXmlPostfix);
        chunkDocument.save_file(filepath.string().c_str());
    }

    auto Chunk::operator==(const gfx::Chunk &other) const -> bool {
        return identifier == other.identifier && shaderModule == other.shaderModule && indices == other.indices &&
               geometry == other.geometry;
    }

    auto Chunk::load(const std::filesystem::path &path) -> std::optional<Chunk> {
        pugi::xml_document chunkDocument;
        pugi::xml_parse_result parseResult = chunkDocument.load_file(path.c_str());
        // If failure, spit out the error
        if (!parseResult) {
#ifndef NDEBUG
            assert(parseResult && "PARSE FAILED FOR CHUNK");
#endif
            spdlog::error("Failed to load project file with error: {}", parseResult.description());
            return std::nullopt;
        }

        // Top-level chunk element
        const pugi::xml_node chunkNode = chunkDocument.child("chunk");
        const std::string identifier = chunkNode.attribute("name").value();
        const std::string moduleName = chunkNode.attribute("shaderModule").value();
        const bool isFixture = std::strcmp(chunkNode.attribute("isFixture").value(), "true") == 0;

        // Dimensions
        const pugi::xml_node dimensionsNode = chunkNode.child("dimensions");
        const pugi::xml_node xdimNode = dimensionsNode.child("xdim");
        const pugi::xml_node ydimNode = dimensionsNode.child("ydim");
        const pugi::xml_node zdimNode = dimensionsNode.child("zdim");

        /* const ivec3 chunkSize(); */
        /* const vec3 chunkTranslation(); */
        /* const std::string moduleName; */
        /* const std::string identifier; */
        /* const bool isFixture; */
        /* const BlockType blockType; */
    }

}// namespace vx::gfx
