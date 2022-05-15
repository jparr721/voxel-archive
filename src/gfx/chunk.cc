#include "chunk.h"
#include "../paths.h"
#include "../util//strings.h"
#include <fstream>
#include <iostream>
#include <pugixml.hpp>
#include <spdlog/spdlog.h>
#include <utility>

constexpr int kChunkFileVersionMajor = 0;
constexpr int kChunkFileVersionMinor = 1;
constexpr int kChunkFileVersionPatch = 0;

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
        setBounds();
        spdlog::debug("Chunk loaded successfully");
    }

    void Chunk::write() const noexcept {
        pugi::xml_document chunkDocument;

        // Top-level chunk
        pugi::xml_node chunkNode = chunkDocument.append_child("chunk");
        chunkNode.append_attribute("name") = identifier.c_str();
        chunkNode.append_attribute("version") =
                util::semverToString(kChunkFileVersionMajor, kChunkFileVersionMinor, kChunkFileVersionPatch).c_str();
        chunkNode.append_attribute("shaderModule") = shaderModule.c_str();

        // Write Dimensions
        pugi::xml_node dimensionsNode = chunkNode.append_child("dimensions");
        pugi::xml_node xdimNode = dimensionsNode.append_child("xdim");
        xdimNode.append_attribute("size") = xdim;
        xdimNode.append_attribute("min") = xmin;
        xdimNode.append_attribute("max") = xmax;

        pugi::xml_node ydimNode = dimensionsNode.append_child("ydim");
        ydimNode.append_attribute("size") = ydim;
        ydimNode.append_attribute("min") = ymin;
        ydimNode.append_attribute("max") = ymax;

        pugi::xml_node zdimNode = dimensionsNode.append_child("zdim");
        zdimNode.append_attribute("size") = zdim;
        zdimNode.append_attribute("min") = zmin;
        zdimNode.append_attribute("max") = zmax;

        // Write the indices of this chunk
        pugi::xml_node indicesNode = chunkNode.append_child("indices");
        indicesNode.append_attribute("n_nodes") = indices.size();

        for (const auto &index : indices) {
            pugi::xml_node indexNode = indicesNode.append_child("index");
            indexNode.append_attribute("value") = index;
        }

        // Write the vertices
        pugi::xml_node verticesNode = chunkNode.append_child("vertices");
        verticesNode.append_attribute("n_nodes") = geometry.size();

        int ii = 0;
        for (const auto &vertexColorHex : geometry) {
            const auto &x = vertexColorHex.position.x;
            const auto &y = vertexColorHex.position.y;
            const auto &z = vertexColorHex.position.z;
            const auto &color = vertexColorHex.color;

            // Make a vertex top-level element
            pugi::xml_node vertexNode = verticesNode.append_child("vertex");
            vertexNode.append_attribute("index") = ii;
            vertexNode.append_attribute("x") = x;
            vertexNode.append_attribute("y") = y;
            vertexNode.append_attribute("z") = z;
            vertexNode.append_attribute("blockType") = gfx::blockTypeToString(blockType).c_str();
            vertexNode.append_attribute("vertexType") = "VertexColorHex";
            ++ii;
        }

#ifndef NDEBUG
        // Debug print
        chunkDocument.print(std::cout);
#endif

        // Save the file to our pre-determined path
        const auto filepath = isFixture ? paths::kFixturesPath / fs::path(identifier + paths::kXmlPostfix)
                                        : paths::kGameObjetsPath / fs::path(identifier + paths::kXmlPostfix);
        chunkDocument.save_file(filepath.string().c_str());
    }

    auto Chunk::operator==(const gfx::Chunk &other) const -> bool {
        return identifier == other.identifier && shaderModule == other.shaderModule && xmin == other.xmin &&
               xmax == other.xmax && ymin == other.ymin && ymax == other.ymax && zmin == other.zmin &&
               zmax == other.zmax && indices == other.indices && geometry == other.geometry;
    }

    void Chunk::setBounds() {
        for (const auto &vertex : geometry) {
            const auto x = vertex.position[0];
            const auto y = vertex.position[1];
            const auto z = vertex.position[2];

            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);

            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);

            zmin = std::min(zmin, z);
            zmax = std::max(zmax, z);
        }
    }
}// namespace vx::gfx
