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

    // Path to the resources/assets/fixtures folder
    inline const fs::path kFixturesPath = kAssetsPath / fs::path("fixtures");

    // Path to the resources/assets/game_objects folder
    inline const fs::path kGameObjetsPath = kAssetsPath / fs::path("game_objects");

    // Path to the resources/shaders folder
    inline const fs::path kShadersPath = kResourcesPath / fs::path("shaders");

    enum ShaderModule { kCore = 0, kDebug };
    inline const std::array<char *, 2> kAvailableShaderModules = {(char *) "core", (char *) "debug"};
}// namespace vx::paths
