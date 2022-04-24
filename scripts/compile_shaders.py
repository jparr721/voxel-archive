import os
import platform
import subprocess
from collections import namedtuple

Shaders = namedtuple("Shaders", ["vertex_shader_path", "fragment_shader_path"])


def check_exists(path: str):
    if not os.path.exists(path):
        raise ValueError(f"Path {path} does not exist.")


def get_platform() -> str:
    def is_linux() -> bool:
        return os.name == "posix" and platform.system().lower() != "darwin"

    def is_mac() -> bool:
        return os.name == "posix" and platform.system().lower() == "darwin"

    def is_windows() -> bool:
        return not is_linux() and not is_mac()

    if is_mac():
        system = "mac"
    elif is_windows():
        system = "windows"
    elif is_linux():
        system = "linux"
    else:
        raise ValueError("Platform cannot be deduced. Exiting.")

    return system


def get_shader_library() -> str:
    platform = get_platform()
    if platform == "osx":
        return "metal"
    if platform == "windows":
        return "s_5_0"
    # Vulkan targeting only for now on linux.
    return "spirv"


def get_shaderc_path() -> str:
    platform = get_platform()
    if platform != "windows":
        build_dir = os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "..", "build"
        )
        return os.path.join(build_dir, "third_party", "bgfx", "shaderc")
    else:
        build_dir = os.path.join(
            os.path.dirname(os.path.abspath(__file__)), "..", "build"
        )
        return os.path.join(build_dir, "third_party", "bgfx", "Debug", "shaderc.exe")


def get_shader_paths() -> Shaders:
    resources_dir = os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "resources"
    )
    shader_core_dir = os.path.join(resources_dir, "shaders", "core")
    vertex_shader_path = os.path.join(shader_core_dir, "core.vs.sc")
    fragment_shader_path = os.path.join(shader_core_dir, "core.fs.sc")
    return Shaders(vertex_shader_path, fragment_shader_path)


def get_varyingdef_path() -> str:
    vs_path, _ = get_shader_paths()
    return os.path.join(os.path.dirname(vs_path), "varying.def.sc")


def get_output_shader_paths() -> Shaders:
    vertex_shader_path, fragment_shader_path = get_shader_paths()

    def expand_path(pathvar: str) -> str:
        platform = get_platform()

        lib = get_shader_library() if platform != "windows" else "dx11"

        filename = os.path.basename(pathvar)
        path = os.path.dirname(pathvar)
        return os.path.join(path, lib, f"{filename}.bin")

    return Shaders(expand_path(vertex_shader_path), expand_path(fragment_shader_path))


def get_shader_type(path: str) -> str:
    if "vs.sc" in path:
        return "v"
    if "fs.sc" in path:
        return "f"
    raise ValueError(f"Invalid shader: {path}")


def compile_shader_file(
    shaderc_path: str,
    shader_path: str,
    shader_out_path: str,
    shader_varying_path: str,
    system: str,
    shader_library: str,
) -> None:
    subprocess.call(
        [
            f"{shaderc_path}",
            "-f",
            shader_path,
            "-o",
            shader_out_path,
            "--type",
            get_shader_type(shader_path),
            "-p",
            shader_library,
            "--platform",
            system,
            "--varyingdef",
            shader_varying_path,
        ]
    )


def compile_shaders():
    system = get_platform()
    shader_library = get_shader_library()
    shaderc_path = get_shaderc_path()
    varying_path = get_varyingdef_path()
    vs_path, fs_path = get_shader_paths()
    c_vs_path, c_fs_path = get_output_shader_paths()

    check_exists(shaderc_path)
    check_exists(varying_path)
    check_exists(vs_path)
    check_exists(fs_path)

    print("===================SYSTEM==================")
    print(f"Using OS: {system}")
    print(f"Using graphics engine: {shader_library}")
    print(f"Using shaderc path: {shaderc_path}")
    print(f"Using varyingdef path: {varying_path}")
    print(f"Using resource path: {vs_path}, \n\t{fs_path}")
    print(f"Using compiled resource path: {c_vs_path}, \n\t{c_fs_path}")
    print("=========================================")
    print("Beginning Shader Compilation")

    # Make sure the output dir exists
    if not os.path.exists(os.path.dirname(c_vs_path)):
        os.mkdir(os.path.dirname(c_vs_path))
        print(f"Make output directory at: {os.path.dirname(c_vs_path)}")

    print(f"Compiling: {vs_path}")
    compile_shader_file(
        shaderc_path, vs_path, c_vs_path, varying_path, system, shader_library
    )
    print(f"Compiling: {fs_path}")
    compile_shader_file(
        shaderc_path, fs_path, c_fs_path, varying_path, system, shader_library
    )

    check_exists(c_vs_path)
    check_exists(c_fs_path)


if __name__ == "__main__":
    compile_shaders()
    print("Compilation completed without any errors")
