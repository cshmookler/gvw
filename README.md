# GLFW and Vulkan Wrapper
A C++ library for creating cross-platform desktop applications with GLFW and Vulkan. Started development on November 24th, 2022.
### TODO
 * [X] Setup CMake
 * [X] Create a window using GLFW
 * [X] Finish most GLFW infrastructure
 * [ ] Setup Doxygen and add comments
 * [ ] Create tests
 * [ ] Create the necessary infrastructure to draw a triangle
 * [ ] (optional) Save space in the code by loading settings from a file
 * [ ] Make the triangle move
 * [ ] Draw two triangles to form a square
 * [ ] Move the square with the keyboard
 * [ ] Draw text on the screen
 * [ ] Implement rigid-body physics (collisions)
 * [ ] Create a simple 2D platformer
 * [ ] Draw a 3D cube
 * [ ] Rotate the cube
### Build this project from source (for Unix-like systems using X11)
**1.** Open a terminal window in the root directory of this project.

**2.** Install the required packages for building this project.
```bash
$ sudo apt install g++ cmake xorg-dev
```
**3.** Enter the `external` directory. There should be three files named `cpplocate-2.3.0.zip`, `glfw-3.3.8.zip`, and `stb-master.zip`. Unzip all of them.
```bash
$ cd external
$ unzip "*.zip"
```
**4.** Enter the `glfw-3.3.8` directory. Then create a directory named `build` and enter it.
```bash
$ cd glfw-3.3.8
$ mkdir build && cd build
```
**5.** Build and install glfw.
```bash
$ cmake -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF ..
$ sudo make install
```
**6.** Enter the `cpplocate-2.3.0` directory. Then create a directory named `build` and enter it.
```bash
$ cd ../../cpplocate-2.3.0
$ mkdir build && cd build
```
**7.** Build and install cpplocate.
```bash
$ cmake -D OPTION_BUILD_TESTS=OFF ..
$ sudo make install
```
**8.** Go back to the root directory of this project. Then create a directory named `build` and enter it.
```bash
$ cd ../../..
$ mkdir build && cd build
```
**9.** Generate CMake information for the build system. By default, this project is built in release mode as a static library with a few example programs.
```bash
$ cmake ..
```
For building a shared (dynamic) library instead of a static one, set the `GVW_BUILD_SHARED_LIBS` flag to ON.
```bash
$ cmake -DGVW_BUILD_SHARED_LIBS=ON ..
```
For debug builds, set the `CMAKE_BUILD_TYPE` flag to Debug.
```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```
For not building the example programs, set the `GVW_BUILD_EXAMPLES` flag to OFF.
```bash
$ cmake -DGVW_BUILD_EXAMPLES=OFF ..
```
Any of the above flags can be combined into one command. For example, the below command generates CMake info for compiling a shared library in debug mode without example programs.
```bash
$ cmake -DGVW_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug -DGVW_BUILD_EXAMPLES=OFF ..
```
**10.** Finally, build the project.
```bash
$ make
```
To install the library after compilation, run this instead:
```bash
$ sudo make install
```
