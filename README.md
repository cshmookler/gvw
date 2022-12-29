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
1. Install the required packages for c++, glfw, and vulkan. You will also need to install conan and cmake.
```bash
$ sudo apt install g++ libglfw3-dev libstb-dev vulkan-sdk
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
5. (For Release builds) Generate CMake information for the build system.
```bash
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```
6. (For Debug builds) Generate CMake information for the build system.
```bash
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
```
7. Finally, build the project.
```bash
$ make
```
8. Run the generated executable.
```bash
$ ./bin/vulkan_template
```
