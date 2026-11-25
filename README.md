# GAMES101 — Modernized Skeleton

A modernized fork of [Jinof/GAMES101-S2021-Skeleton](https://github.com/Jinof/GAMES101-S2021-Skeleton),
the official code framework for GAMES101 (Modern Computer Graphics, Lingqi Yan).

The assignment content is unchanged — every `TODO` you are asked to implement is
preserved verbatim, with the same function signatures the handouts describe.
What changed is everything around it:

- **C++20** throughout: `#pragma once`, `std::numbers`, `enum class`,
  `[[nodiscard]]`, RAII ownership (`std::unique_ptr`), `std::optional` for
  ray intersections (PA6/PA7), `std::jthread` rendering (PA7).
- **Modern CMake** (>= 3.25): target-based linking only, no hardcoded
  `include_directories(/usr/local/include)`, strict warnings on every target.
- **vcpkg manifest mode** for Eigen + OpenCV (`vcpkg.json`), with
  **CMake presets** so one command configures and builds on Windows,
  macOS (Apple Silicon) and Linux.
- Asset paths resolved at compile time — binaries find their models and
  textures no matter which directory they are launched from.

Assignment 8 (the CGL mass-spring viewer borrowed from CS184) and the Final
Project are kept for reference but are **not** part of the build; they are a
separate framework and a separate work line.

## Prerequisites

- CMake >= 3.25 and a C++20 compiler (MSVC 2022, Apple Clang 15+, GCC 12+ / Clang 15+)
- [vcpkg](https://github.com/microsoft/vcpkg) with the `VCPKG_ROOT`
  environment variable pointing at its root — **or** system packages
  (see the `sys-*` presets below)

## Build

```bash
# Windows (MSVC)
cmake --preset win
cmake --build --preset win

# macOS (Apple Silicon)
cmake --preset mac
cmake --build --preset mac

# Linux
cmake --preset linux
cmake --build --preset linux
```

The first vcpkg configure builds OpenCV from source and can take a while;
later configures reuse the cache.

### Without vcpkg (system packages)

```bash
# Ubuntu / Debian
sudo apt install libeigen3-dev libopencv-dev
cmake --preset sys-linux && cmake --build --preset sys-linux

# macOS (Homebrew)
brew install eigen opencv
cmake --preset sys-mac && cmake --build --preset sys-mac
```

## Targets

| Target | Assignment |
| ------ | ---------- |
| `pa1`  | Rasterizing triangles (wireframe) |
| `pa2`  | Rasterizing triangles and z-buffering |
| `pa3`  | Shading: texture, Blinn-Phong, bump & displacement |
| `pa4`  | Bézier curves |
| `pa5`  | Ray tracing: Whitted-style |
| `pa6`  | Bounding volume hierarchies |
| `pa7`  | Path tracing |

Run a binary from its build output directory, e.g.
`./out/build/linux/Assignment1/pa1 -r 45 output.png`.

## Layout

```
Assignment1/ ... Assignment7/   modernized frameworks, one target each
Assignment8/                    original CGL framework (reference only)
*.pdf                           original assignment handouts
CMakePresets.json               win / mac / linux / sys-* presets
vcpkg.json                      eigen3 + opencv4
```
