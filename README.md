# game-engine

Game engine built in C99 using [raylib](https://github.com/raysan5/raylib) and Lua scripting for mods

Features:
- Cross platform
    - Runs on Linux, Windows, and the web
    - Builds on Linux can target multiple platforms
- Easy modding and extensible
    - Game logic is defined as mods in Lua scripts
    - Lua metatables provide intellisense and completion suggestions for LSPs
    - Hooks can be user-defined at runtime, and optionally registered
    - Event system allows for messaging and communication within the engine
- Hot-reloading
    - Modifications to Lua files will see changes reflected in real-time
    - On POSIX systems, scenes can be loaded at runtime as shared libraries (.so files)

## Building

### Arch Linux

The prerequisite packages for building are:

```bash
sudo pacman -S --needed git gcc cmake ninja luarocks
```

```bash
## Optionally pass the -d flag to build for debug
./build.sh
```

And run the example project with the run script
```
./run.sh test-game
```

#### Web builds with Emscripten

Web builds use [Emscripten](https://github.com/emscripten-core/emscripten), and need the following additional packages:

```bash
sudo pacman -S --needed emscripten
```

Build by passing in a new target flag to the build script

```bash
## Optionally pass the -d flag to build for debug
./build.sh -t web
```

And run the web build with the run script
```bash
./run.sh -t web holojam
```

> [!WARNING]  
> Currently, the `test-game` project does not run with Emscripten due to ASYNCIFY stack corruption when using `dlopen()` and `dlsym()` to load and run functions from a shared library file

#### Windows builds with Wine

Windows builds use Wine to run Microsoft's MSVC compilter, through the [msvc-wine](https://github.com/mstorsjo/msvc-wine) project.

For Arch Linux, the prerequisite packages are,

```bash
sudo pacman -S --needed git gcc make cmake wine msitools samba python python-simplejson python-six
```

To install the MSVC compiler, we can clone the repository and run the install scripts

```bash
git clone https://github.com/mstorsjo/msvc-wine.git
cd msvc-wine

# This example installs the compiler to ~/my_msvc/opt/msvc
./vsdownload.py --dest ~/my_msvc/opt/msvc
./install.sh ~/my_msvc/opt/msvc

# Add compiler commands to PATH
export PATH=~/my_msvc/opt/msvc/bin/x64:$PATH

# Optional: Start a persistent wineserver
wineserver -k # Kill a potential old server
wineserver -p # Start a new server
wine64 wineboot # Run a process to start up all background wine processes
```

Build by passing in a new target flag to the build script

```bash
./build.sh -t windows
```

> [!NOTE]  
> Debug builds will not work when targeting Windows in Wine, and will give errors for various missing DLLs

And run the windows build with the run script

```bash
./run.sh -t windows holojam
```

> [!WARNING]  
> Currently, the `test-game` project only builds and runs properly on POSIX systems, due to the global symbol table not being shared with functions from shared libraries in Windows

## Building for release

Releases builds are done in a special release directory and the specified release files are copied into the `release[-<project-type>]` directory

```bash
# build files are located in build-release/ and releases are copied to release/
./build.sh --release
```

```bash
# build files are located in build-windows-release/ and releases are copied to release-windows/
./build.sh -t windows --release
```

Release builds are meant to be the final version of your software that is shipped, so make sure all paths are valid when running from the right directory and remember to include all relevant asset files

> [!NOTE]
> The `release` directory has its file structure determined by a project's CMake config. This is typically done by copying specific files and directories to specific locations with post-build custom commands and CMake variables. Refer to the `holojam` project's CMake files for an example of how its release is configured.

## Libraries

- https://github.com/raysan5/raylib Simple graphics library
- https://github.com/raysan5/rres File-format for resource-packaging
- https://github.com/Immediate-Mode-UI/Nuklear Minimal intermediate-mode UI library
- https://github.com/RobLoach/raylib-nuklear Wrapper of Nuklear for Raylib
- https://github.com/zfletch/zhash-c Hash table library
- https://github.com/json-c/json-c JSON library
- https://github.com/walterschell/Lua CMake based build of Lua
- https://github.com/emscripten-core/emscripten LLVM-to-WASM compiler
- https://github.com/Keyslam-Group/Concord Feature compute Lua ECS
- https://github.com/yogeshlonkar/lua-import Relative imports for Lua
