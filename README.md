# voxel
Voxel.

## Contributing
Make sure you clone this repository with the submodules, if you forgot to do that, you'll need to run `git submodule update --init --recursive` to get the dependencies. Almost everything here is statically linked, so you should be good with system dependencies.

### Running
If you're on Windows, you'll need to use CMake to generate a Visual Studio solution, otherwise, if you are going the CMake route, just open it in a CMake-Supported editor.

If you're on Unix, you can do the following:
```sh
$ cd <project_root>
$ mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make -jX
```
You should now have a `voxel` executable. Right now, this system will fail if you haven't compiled the shaders. There is currently not an automatic check for this, but never fear! It's really easy to compile the shaders. 

```sh
$ cd <project_root>
$ python scripts/compile_shaders.py
```
This will compile the shaders for your target architecture and platform. If there are still issues, fix them.

You can also just use an editor like vscode to build the project for you if you don't feel like doing it manually, but you'll still need to potentially compile the shaders. An automatic recompilation routine will be spun up eventually.
