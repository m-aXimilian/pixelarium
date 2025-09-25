
# Synopsis

Pixelarium strives to be a batteries-included visualizer application used in conjunction with an externally implemented and linked arbitrary functionality.
It can be linked e.g. against a library containing arbitrary functionality. Pixelarium can support viewing the results and result files of such a library.
It tries to be as flexible as possible.

This is still work in progress and will change significantly.


# Prerequisites

Dependencies are either submodules in the `modules` subdirectory or artifacts of the cmake build process from the `cmake` directory. This repository should therefore be cloned recursively:

    git clone --recurse-submodules https://github.com/m-aXimilian/pixelarium.git

Apart from that, this project needs OpenCV installed on the host system and available for cmake's `find_package`.


# Building

Given that the prerequisites are fulfilled, building can be achieved via one of the presets or by calling cmake directly.


## Presets

Pixelarium has a few presets setting specific compilers and configurations defined in `CMakePresets.json`.

They can be listed by calling

    cmake --list-presets

which will give something like

    Available configure presets:
    
      "clang-release"
      "clang-debug"
      "gcc-release"
      "gcc-debug"

Building with the `clang-debug` preset would look like

    cmake --preset clang-debug
    cmake --build --preset clang-debug


## Direct

If you want to specify compiler settings and options which are not defined in a preset, use cmake "directly" like

    cmake -B build -S .
    cmake --build build



