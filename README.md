# game-engine

Game engine built in C99 using [raylib](https://github.com/raysan5/raylib) and Lua scripting for mods

## Building

### Arch Linux

The prerequisite packages for building are:

```bash
sudo pacman -S --needed git gcc cmake
```
[LuaJIT](https://luajit.org/) is required to build the project, and its static libraries should be installed following the build instructions on the site

[json-c](https://github.com/json-c/json-c/wiki#building) is also required to build the project, so you should build and install its static libraries following the build instructions on the Github page

With the packages installed, the build script can be run to compile the project

```bash
## Optionally pass the -d flag to build for debug
./build.sh
```

And run the example project with the run script
```
./run.sh test-game
```
