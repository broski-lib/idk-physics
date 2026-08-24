# idk-physics

C++ scaffold: a simulation core with an optional ImGui + ImPlot3D UI that runs
as a secondary consumer of the sim's state.

## Layout

- `src/sim/` — pure simulation logic, no rendering or UI dependencies
- `src/ui/` — GLFW window, ImGui and ImPlot3D; reads the sim snapshot only
- `src/app/` — main flow: steps the sim at a fixed rate, drives the UI
- `config/idk_imconfig.h` — ImGui user config (32-bit draw indices)

Dependencies (ImGui v1.92.9, ImPlot3D v0.4, GLFW 3.4) are fetched by CMake.

## Build

```sh
./build.sh          # configure + build into build/
./build/idk_physics
```

Run without the UI (headless sim):

```sh
BUILD_DIR=build-headless ./build.sh -DBUILD_UI=OFF
./build-headless/idk_physics
```

CMake options and extra flags can be passed through (e.g. `-DCMAKE_BUILD_TYPE=Debug`).