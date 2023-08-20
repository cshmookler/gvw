# **GLFW and Vulkan Wrapper**
A C++ library for creating cross-platform desktop applications with GLFW and Vulkan. Started development on November 24th, 2022.

### **Build from source and install using Conan 2 (for Unix-like systems using X11)**
**1.** Install a C++ compiler (Example: g++), the LunarG Vulkan SDK, Git, Python 3.6+, and the Python Virtual Environment using the system package manager (Example: apt).
```bash
$ sudo apt install g++ git python3 python3-venv
```
**2.** Verify that your version of python is >= 3.6.
```bash
$ python3 --version
```
**3.** Create a Python 3.6+ virtual environment and activate it.
```bash
$ python3 -m venv venv
$ source venv/bin/activate
```
**4.** Install Conan 2.
```bash
$ pip3 install "conan>=2.0.0"
```
**5.** Verify that Conan 2 was successfully installed.
```bash
$ conan --version
```
**6.** Create the default Conan profile. Ensure that it is configured to use C++20 or higher.
```bash
$ conan profile detect
```
**7.** Clone the GVW git repository and enter the root directory of the project.
```bash
$ git clone https://github.com/cshmookler/GLFW-and-Vulkan-wrapper.git
$ cd GLFW-and-Vulkan-wrapper
```
**8.** Build and install this project with Conan 2. Change the last configuration option (`tools.system.package_manager:tool`) to your default system packet manager (options: apt-get, yum, dnf, brew, pacman, choco, zypper, pkg, or pkgutil). To build in debug mode, change the last option (`build_type`) to Debug.
```bash
$ conan create . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True -c tools.system.package_manager:tool=apt-get --build=missing -s build_type=Release
```
To build this project with Conan 2 without installing it, swap out `conan create` with `conan build`.
```bash
$ conan build . -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True -c tools.system.package_manager:tool=apt-get --build=missing -s build_type=Release
```

### **Technical TODO**
 * [ ] Create enums for GLFW integer values.
 * [X] Replace namespace-like classes with the types they represent.
 * [ ] Further compartmentalize Vulkan functions and permit more customization at runtime.
 * [ ] Replace Vulkan-Hpp with Vulkan to improve compiletime (?).
 * [ ] Ensure thread-safety for GLFW functions.

### **Aspirational TODO**
 * [X] Setup CMake.
 * [X] Create a window using GLFW.
 * [X] Finish most GLFW infrastructure.
 * [X] Setup Doxygen and add comments.
 * [X] Implement support for Conan 2.0.
 * [X] Create the necessary infrastructure to draw a triangle.
 * [x] Draw two triangles to form a square.
 * [x] Make the triangle move.
 * [x] Move the square with the keyboard
 * [ ] Draw text
 * [ ] Draw GUI elements
 * [ ] Implement rigid-body physics (collisions)
 * [ ] Create a simple 2D platformer
 * [ ] Draw a 3D cube
 * [ ] Rotate the cube
