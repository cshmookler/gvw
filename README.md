# **GLFW and Vulkan wrapper**
A thick wrapper for GLFW and Vulkan. Started development on November 24th, 2022.
## **TODO**
 * [X] Build the project with cmake
 * [X] Create a window using GLFW
 * [ ] Finish GLFW infrastructure
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
## **Build this project**
1. Install the required packages for C++, GLFW, and Vulkan. You will also need to install conan, cmake, and the vulkan sdk.
```bash
$ sudo apt install g++ libglfw3-dev libstb-dev
```
2. Open a terminal window in the root directory of this project.
3. Create and enter the build directory.
```bash
$ mkdir build
$ cd build
```
4. Generate Conan information for the build system.
```bash
$ conan install ..
```
5. Generate CMake information for the build system. By default, this project is built in release mode as a static library with a few example programs.
```bash
$ cmake ..
```
For building a shared (dynamic) library instead of a static one, set the GVW_BUILD_SHARED_LIBS flag to ON.
```bash
$ cmake -DGVW_BUILD_SHARED_LIBS=ON ..
```
For debug builds, set the CMAKE_BUILD_TYPE flag to Debug.
```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```
For not building the example programs, set the GVW_BUILD_EXAMPLES flag to OFF.
```bash
$ cmake -DGVW_BUILD_EXAMPLES=OFF ..
```
Any of the above flags can be combined into one command. For example, the below command generates CMake info for compiling a shared library in debug mode without example programs.
```bash
$ cmake -DGVW_BUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Debug -DGVW_BUILD_EXAMPLES=OFF ..
```
6. Finally, build the project.
```bash
$ make
```
To install the library after compilation, use 'sudo make install' instead.
```bash
$ sudo make install
```
