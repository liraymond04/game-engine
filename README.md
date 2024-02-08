# game-engine

Game engine built in C99 using [raylib](https://github.com/raysan5/raylib) and Lua scripting for mods

## Building

### Arch Linux

The prerequisite packages for building are:

```bash
sudo pacman -S --needed git gcc cmake ninja
```

```bash
## Optionally pass the -d flag to build for debug
./build.sh
```

And run the example project with the run script
```
./run.sh test-game
```
