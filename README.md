# Bullet Manager

Here is my test task. The desired class can be found in `Source/Client/Managers/`. Besides that, you can find simple engine implementation and game on it, using `BulletManager`.

External dependencies for the project:
* SDL2 as for window creation and input handling.
* Dear imgui for application UI rendering.
* gl3w for loading Open Gl functions.

Below, you can find the build instructions. Don't forget to pull data from lfs and submodules init before build! (But, if you received this file in a zip archive, you don't have to do these steps)
$ git submodule update --init --recursive
$ git lfs install
$ git lfs pull

#### Windows (Using VS generator)
$ CMake -G "Visual Studio 16 2019"
After that, run generated solution
