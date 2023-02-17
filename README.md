# Simple Bullet Manager

My simple 2d rendering engine with simple implementation of collision detection system.

External dependencies for the project:
* SDL2 as for window creation and input handling.
* Dear imgui for application UI rendering.
* gl3w for loading Open Gl functions.

Below, you can find the build instructions. Don't forget to pull data from lfs and submodules init before build!
```
$ git submodule update --init --recursive
$ git lfs install
$ git lfs pull
```

#### Windows (Using VS generator)
```
$ CMake -G "Visual Studio 16 2019"
```
After that, run generated solution

#### MacOS (Not tested)
```
$ CMake -G Xcode
```
