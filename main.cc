#include "src/window.h"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    spdlog::info("Logging level is at info");
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::info("Logging level is at debug");
    spdlog::set_level(spdlog::level::debug);
#endif
    vx::launchWindow("Voxel");
}