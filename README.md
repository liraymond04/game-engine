# game-engine

Game engine built in C99 using [raylib](https://github.com/raysan5/raylib) and Lua scripting for mods

## Building

### Arch Linux

The prerequisite packages for building are:

```bash
sudo pacman -S --needed git gcc cmake luajit
```

[json-c](https://github.com/json-c/json-c/wiki#building) is also required to build the project, so you should build and install it following the build instructions on the Github page

With the packages installed, the build script can be run to compile the project

```bash
## Optionally pass the -d flag to build for debug
./build.sh
```

And run the example project with the run script
```
./run.sh test-game
```
