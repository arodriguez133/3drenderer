# 3D Renderer

A lightweight software 3D renderer written in C using SDL2 for display.

![Demo](docs/demo.gif)

## Features

- Software rasterization (no GPU required)
- 3D vector math with X, Y, Z axis rotation
- Perspective projection
- SDL2 windowed display

## Dependencies

- [SDL2](https://www.libsdl.org/)

## Building

**Linux:**
```bash
cd src
gcc -Wall -std=c99 *.c -lSDL2 -o renderer
./renderer
```

**macOS (Homebrew):**
```bash
brew install sdl2
cd src
SDL2_PREFIX=$(brew --prefix sdl2)
gcc -Wall -std=c99 *.c -I"$SDL2_PREFIX/include" -L"$SDL2_PREFIX/lib" -lSDL2 -o renderer
./renderer
```

**Windows (MSYS2/MinGW64):**
```bash
cd src
gcc -Wall -std=c99 *.c $(sdl2-config --cflags --libs) -o renderer.exe
./renderer.exe
```

## Controls

| Key | Action |
|-----|--------|
| `Esc` | Quit |

## Downloads

Pre-built binaries for Linux, Windows, and macOS are available on the [Releases](../../releases) page.
