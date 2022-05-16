#pragma once

#include <array>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// We break the namespace rule here to allow for a reduction in name collisions.
namespace vx::paths {
    inline const char *pathToPathsFile = __FILE__;
    // Path to the resources folder
    inline const fs::path kResourcesPath =
            fs::path(pathToPathsFile).parent_path().parent_path() / fs::path("resources");

    // Path to the resources/assets folder
    inline const fs::path kAssetsPath = kResourcesPath / fs::path("assets");

    // Path to the resources/assets/projects folder
    inline const fs::path kProjectsPath = kAssetsPath / fs::path("projects");

    // Path to the resources/shaders folder
    inline const fs::path kShadersPath = kResourcesPath / fs::path("shaders");

    // Standard extension for projects
    inline constexpr auto kXmlPostfix = ".xml";

    // Shader modules
    enum ShaderModule { kCore = 0, kDebug };
    inline const std::array<char *, 2> kAvailableShaderModules = {(char *) "core", (char *) "debug"};
}// namespace vx::paths
