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
    Chunk::Chunk(const ivec3 &chunkSize, const vec3 &chunkTranslation, std::string moduleName, std::string _name,
                 bool _isFixture, const BlockType &_blockType)
        : shaderModule(std::move(moduleName)), name(std::move(_name)), isFixture(_isFixture) {

        // Generates a random uuid
        std::random_device rd;
        auto seed_data = std::array<int, std::mt19937::state_size>{};
        std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::mt19937 generator(seq);
        uuids::uuid_random_generator gen{generator};
        id = gen();
        //

        spdlog::debug("Loading chunk id: {} module: {}", uuids::to_string(id), shaderModule);
        setGeometry(chunkSize, chunkTranslation, _blockType);
        spdlog::debug("Chunk loaded successfully");
    }

    void Chunk::write() const noexcept {
        pugi::xml_document chunkDocument;
        pugi::xml_node projectNode = level_editor::Project::instance()->projectXMLHeader(chunkDocument);

        pugi::xml_node identifierComponent = projectNode.append_child("component");
        identifierComponent.append_attribute("name") = "id";
        identifierComponent.append_attribute("value") = uuids::to_string(id).c_str();

        pugi::xml_node nameComponent = projectNode.append_child("component");
        nameComponent.append_attribute("name") = "name";
        nameComponent.append_attribute("value") = name.c_str();

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

        pugi::xml_node transformComponent = projectNode.append_child("component");
        transformComponent.append_attribute("name") = "transform";
        pugi::xml_node transformMapComponent = transformComponent.append_child("map");
        pugi::xml_node xtransformEntry = transformMapComponent.append_child("entry");
        xtransformEntry.append_attribute("key") = "xtransform";
        xtransformEntry.append_attribute("value") = xtransform;

        pugi::xml_node ytransformEntry = transformMapComponent.append_child("entry");
        ytransformEntry.append_attribute("key") = "ytransform";
        ytransformEntry.append_attribute("value") = ytransform;

        pugi::xml_node ztransformEntry = transformMapComponent.append_child("entry");
        ztransformEntry.append_attribute("key") = "ztransform";
        ztransformEntry.append_attribute("value") = ztransform;


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
        verticesComponentVertexTypeProperty.append_attribute("value") = "VertexColor";

        pugi::xml_node verticesList = verticesComponent.append_child("vertices-list");
        int ii = 0;
        for (const auto &[position, _color] : geometry) {
            const auto &x = position.x;
            const auto &y = position.y;
            const auto &z = position.z;

            // Make a vertex top-level element
            pugi::xml_node vertexNode = verticesList.append_child("item");
            vertexNode.append_attribute("index") = ii;
            vertexNode.append_attribute("xpos") = x;
            vertexNode.append_attribute("ypos") = y;
            vertexNode.append_attribute("zpos") = z;
            ++ii;
        }

        // Save the file to our pre-determined path
        const auto filepath =
                isFixture ? level_editor::Project::instance()->fixtureFolderPath() / fs::path(name + paths::kXmlPostfix)
                          : level_editor::Project::instance()->gameObjectFolderPath() /
                                    fs::path(name + paths::kXmlPostfix);
        chunkDocument.save_file(filepath.string().c_str());
    }

    void Chunk::setGeometry(const ivec3 &chunkSize, const vec3 &chunkTranslation, const BlockType &_blockType) {
        // Clear any existing memory.
        geometry.clear();
        indices.clear();

        // Set block type
        blockType = _blockType;

        xdim = chunkSize.x;
        ydim = chunkSize.y;
        zdim = chunkSize.z;

        xtransform = chunkTranslation.x;
        ytransform = chunkTranslation.y;
        ztransform = chunkTranslation.z;

        // Origin point is always 0 0 0, so we draw from there
        u64 ii = 0;
        for (int xx = 0; xx < chunkSize.x; ++xx) {
            for (int yy = 0; yy < chunkSize.y; ++yy) {
                for (int zz = 0; zz < chunkSize.z; ++zz) {
                    auto baseIndices = kBlockIndices;

                    // Increment indices to avoid overlapping faces
                    for (auto &index : baseIndices) {
                        index += kCubeVertices.size() * ii;
                        indices.push_back(index);
                    }

                    const vec4 color = makeColorFromBlockType(blockType);
                    const vec3 startingPosition = vec3(xx, yy, zz);
                    for (const auto &vertex : makeOffsetCubeVertices(startingPosition)) {
                        geometry.emplace_back(vertex, color);
                    }
                    ++ii;
                }
            }
        }

        // Translate the chunk.
        for (auto &[pos, _] : geometry) { pos += chunkTranslation; }

        // Write the new data.
        write();
    }

    auto Chunk::operator==(const gfx::Chunk &other) const -> bool { return id == other.id; }

    auto Chunk::load(const std::filesystem::path &path) -> std::optional<Chunk> {
        spdlog::debug("Loading chunk path {}", path.string());
        pugi::xml_document chunkDocument;
        pugi::xml_parse_result parseResult = chunkDocument.load_file(path.c_str());
        // If failure, spit out the error
        if (!parseResult) {
            spdlog::error("Failed to load project file with error: {}", parseResult.description());
#ifndef NDEBUG
            assert(parseResult && "PARSE FAILED FOR CHUNK");
#endif
            return std::nullopt;
        }

        // Top-level chunk element
        const pugi::xml_node projectNode = chunkDocument.child("project");

        spdlog::debug("Loading Identifier Component");
        const pugi::xml_node identifierComponent = projectNode.child("component");

        // TODO(@jparr721) BAD CODE
        const uuids::uuid identifier = uuids::uuid::from_string(identifierComponent.attribute("value").value()).value();

        spdlog::debug("Loading Name Component");
        const pugi::xml_node nameComponent = identifierComponent.next_sibling();
        const std::string name = nameComponent.attribute("value").value();

        spdlog::debug("Loading Shader Module Component");
        const pugi::xml_node shaderModuleComponent = nameComponent.next_sibling();
        const std::string shaderModule = shaderModuleComponent.attribute("value").value();

        spdlog::debug("Loading Fixture Component");
        const pugi::xml_node isFixtureComponent = shaderModuleComponent.next_sibling();
        const bool isFixture = std::strcmp(isFixtureComponent.attribute("value").value(), "true") == 0;

        spdlog::debug("Loading Dimensions");
        ivec3 dimensions;
        const pugi::xml_node dimensionsComponent = isFixtureComponent.next_sibling();
        {
            const pugi::xml_node mapNode = dimensionsComponent.child("map");
            // Unroll the dimensions
#pragma unroll
            for (const auto &child : mapNode.children()) {
                if (std::strcmp(child.attribute("key").value(), "xdim") == 0) {
                    dimensions.x = std::stoi(child.attribute("value").value());
                }

                if (std::strcmp(child.attribute("key").value(), "ydim") == 0) {
                    dimensions.y = std::stoi(child.attribute("value").value());
                }

                if (std::strcmp(child.attribute("key").value(), "zdim") == 0) {
                    dimensions.z = std::stoi(child.attribute("value").value());
                }
            }
        }

        spdlog::debug("Loading Transform");
        const pugi::xml_node transformComponent = dimensionsComponent.next_sibling();
        vec3 transform;
        {
            const pugi::xml_node mapNode = transformComponent.child("map");
            // Unroll the transform
#pragma unroll
            for (const auto &child : mapNode.children()) {
                if (std::strcmp(child.attribute("key").value(), "xtransform") == 0) {
                    transform.x = std::stoi(child.attribute("value").value());
                }

                if (std::strcmp(child.attribute("key").value(), "ytransform") == 0) {
                    transform.y = std::stoi(child.attribute("value").value());
                }

                if (std::strcmp(child.attribute("key").value(), "ztransform") == 0) {
                    transform.z = std::stoi(child.attribute("value").value());
                }
            }
        }

        // Unpack the indices
        spdlog::debug("Loading Indices");
        std::vector<BlockIndexSize> indices;
        const pugi::xml_node indicesComponent = transformComponent.next_sibling();
        {
            const int nNodes = std::stoi(indicesComponent.child("property").attribute("value").value());
            const pugi::xml_node indicesList = indicesComponent.child("indices-list");
            for (const auto &child : indicesList.children()) {
                // Might cause a truncation issue?
                indices.push_back(std::stoi(child.attribute("index").value()));
            }

#ifndef NDEBUG
            assert(indices.size() == nNodes && "INVALID NODE LOAD OPERATION");
#endif
        }

        // Unpack the vertices
        spdlog::debug("Loading Vertices");
        std::vector<VertexColor> vertices;
        const pugi::xml_node verticesComponent = indicesComponent.next_sibling();
        const pugi::xml_node nNodesProperty = verticesComponent.child("property");
        const int nNodes = std::stoi(nNodesProperty.attribute("value").value());
        const pugi::xml_node blockTypeProperty = nNodesProperty.next_sibling();
        const BlockType blockType = stringToBlockType(blockTypeProperty.value());
        {

            // Ignore for now since we are currently only using one vertex type
            const pugi::xml_node vertexTypeProperty = blockTypeProperty.next_sibling();
            const pugi::xml_node verticesList = vertexTypeProperty.next_sibling();
            vec4 color = makeColorFromBlockType(blockType);
            int ii = 0;
            for (const auto &child : verticesList.children()) {
                const u32 xpos = std::stoi(child.attribute("xpos").value());
                const u32 ypos = std::stoi(child.attribute("ypos").value());
                const u32 zpos = std::stoi(child.attribute("zpos").value());
                const vec3 position(xpos, ypos, zpos);

                // Evil hack to get the colors right
                if (ii % gfx::kCubeVertices.size() == 0) { color = makeColorFromBlockType(blockType); }
                vertices.emplace_back(position, color);
                ++ii;
            }
#ifndef NDEBUG
            assert(vertices.size() == nNodes && "INVALID VERTEX LOAD OPERATION");
#endif
        }

        return Chunk(isFixture, blockType, name, shaderModule, identifier, dimensions.x, dimensions.y, dimensions.z,
                     transform.x, transform.y, transform.z, indices, vertices);
    }

}// namespace vx::gfx
