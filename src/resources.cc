#include "resources.h"
#include <filesystem>
#include <fstream>

namespace vx {
    auto loadShader(const std::string &basePath, const std::string &moduleName, const std::string &shaderName)
            -> bgfx::ShaderHandle {
        std::stringstream path;
        std::string platform;

        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D9:
                platform = "dx9";
                break;
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                platform = "dx11";
                break;
            case bgfx::RendererType::Agc:
            case bgfx::RendererType::Gnm:
                platform = "pssl";
                break;
            case bgfx::RendererType::Metal:
                platform = "metal";
                break;
            case bgfx::RendererType::Nvn:
                platform = "nvn";
                break;
            case bgfx::RendererType::OpenGL:
                platform = "glsl";
                break;
            case bgfx::RendererType::OpenGLES:
                platform = "essl";
                break;
            case bgfx::RendererType::Vulkan:
                platform = "spirv";
                break;
            case bgfx::RendererType::WebGPU:
                platform = "spirv";
                break;
            default:
                // TODO
                break;
        }

        spdlog::debug("Using shader platform: {}", platform);
#ifndef NDEBUG
        std::stringstream compiledResourceFolder;
        compiledResourceFolder << basePath << "/" << moduleName << "/" << platform;

        spdlog::debug("Checking for folder: {}", compiledResourceFolder.str());

        if (!std::filesystem::exists(compiledResourceFolder.str())) {
            spdlog::error("Folder does not exist! Please compile the shaders for this directory");
            std::exit(EXIT_FAILURE);
        }
#endif

        path << basePath << "/" << moduleName << "/" << platform << "/" << shaderName << ".bin";

        std::ifstream input(path.str());
        std::stringstream buf;
        buf << input.rdbuf();

        auto text = buf.str();
        spdlog::debug("Reading file at path: {}", text);

        auto result = bgfx::createShader(bgfx::copy(text.c_str(), text.length()));
        bgfx::setName(result, shaderName.c_str());
        return result;
    }

    auto loadShaderProgram(const std::string &basePath, const std::string &moduleName,
                           const std::string &vertexShaderName, const std::string &fragmentShaderName)
            -> bgfx::ProgramHandle {
        const auto vertexShader = loadShader(basePath, moduleName, vertexShaderName);
        const auto fragmentShader = loadShader(basePath, moduleName, fragmentShaderName);
        return bgfx::createProgram(vertexShader, fragmentShader, true /* Destroy shaders */);
    }
}// namespace vx
