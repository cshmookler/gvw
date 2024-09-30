# **GLFW and Vulkan Wrapper**
A C++ library for creating cross-platform desktop applications with GLFW and Vulkan. Started development on November 24th, 2022.

> NOTE: This project is incomplete. Key features may be missing or broken.

### **Build from source and install using Conan 2 (for Unix-like systems using X11)**
**1.** Install a C++ compiler (Example: g++), Git, Python 3.6+, and the Python Virtual Environment using the system package manager (Example: apt).
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

### **Build from source and install using Conan 2 (for Windows)**
**1.** Download the GVW source code from https://github.com/cshmookler/GLFW-and-Vulkan-wrapper/archive/refs/heads/main.zip and extract it.

**2.** Download and install Python 3.6+ from https://www.python.org/downloads/ (Make sure you add python.exe to your PATH during installation!).

**3.** Download and install a C++ compiler that supports C++20 or higher (Example: MSVC).

**4.** Open a new command prompt window by pressing the super key, typing `cmd` and pressing enter. Then use the `cd` command to navigate to the extracted GVW source code.

**5.** Verify that Python was successfully added to your PATH and has a version >= 3.6.
```shell
> python --version
```
**6.** Create a Python 3.6+ virtual environment and activate it.
```shell
> python -m venv venv
> .\venv\Scripts\Activate.bat
```
**7.** Install Conan 2.
```shell
> pip install "conan>=2.0.0"
```
**8.** Verify that Conan 2 was successfully installed.
```shell
> conan --version
```
**9.** Create the default Conan profile. It should detect the C++ compiler you installed earlier. Ensure that it is configured to use C++20 or higher.
```shell
> conan profile detect
```
**10.** Build and install this project with Conan 2. To build in debug mode, change the last option (`build_type`) to Debug.
```shell
> conan create . --build=missing -s build_type=Release
```
To build this project with Conan 2 without installing it, swap out `conan create` with `conan build`.
```shell
> conan build . --build=missing -s build_type=Release
```

### **GVW Programming Guidelines**
 1. Conform to the formatting rules defined in `.clang-tidy` and `.clang-format`.
 2. Header files (ending with .hpp) must begin with `#pragma once` and a comment providing general information about the file.
 3. Header files may only contain declarations and implementation files may only contain implementations. Type definitions are the only exception.
 4. Public identifiers must be declared within the GVW namespace, but not within a subnamespace. Constant variables representing type configurations are the only exception, but their subnamespace must be directly below GVW and must end with `_config`.
 5. Private identifiers must be declared within a subnamespace of GVW named `internal`.
 6. Documenting public and private identifiers with comments is both recommended and optional.
 7. Prefer RAII over C-style resource management.

### **Technical TODO**
 * [X] Create enums for GLFW integer values.
 * [X] Replace namespace-like classes with the types they represent.
 * [X] Add an image loading library for rendering images.
 * [X] Add input mode and key state functions.
 * [X] Change include structure to resolve undefined template instantiation errors in .ipp files.
 * [ ] Make utils/ansiec compatible with Windows.
 * [ ] Remove trailing includes from include structure.
 * [ ] Reduce the size of class section dividers from three lines to one.
 * [ ] Recreate the hint class to not contain a constant member variable (it makes it copy constructable but not copy assignable).
 * [ ] Limit the use of `friend` as much as possible. This is a hack, and may introduce bugs later if allowed to remain.
 * [ ] Replace gvw/gvw.hpp with src/gvw.hpp. Move interface definition files to gvw/ and keep implementation files in src/.
 * [ ] Implement a PIMPL class structure for all applicable types (use `std::shared_ptr` to store the implementation within the interface. Avoiding dynamic allocation by using `std::aligned_storage` instead of `std::shared_ptr` won't work because it won't allow shallow copying of interfaces and will allow the instance object to be destroyed before dependent types are destroyed).
 * [ ] Split large constructors into several helper functions (examples: instance, window, device, etc).
 * [ ] Completely remove the `GVW_INSTANCE` global variable.
 * [ ] Change the case style to be more consistent with common practices. Change the case of constants to something else because `ALL_CAPS` may clash with pre-processor macros.
 * [ ] Make all global variables private non-static members of the instance implementation. Add public functions to access them in a thread-safe way.
 * [ ] Remove the 'e' prefix from enums and undefine clashing macros (Ex. linux).
 * [ ] (?) Assign generic values to enums and add conversion functions to their actual GLFW values within the implementation.
 * [ ] Remove `glfw_types` test (unnecessary once the previous two tasks are completed.
 * [ ] Hide all GLFW function signatures within the implementation by setting GLFW callbacks declared within the implementation that call public functions passed through the interface.
 * [ ] Ensure that trivial operators are marked noexcept.
 * [ ] (?) Always throw an exception if a precondition for a constructor is not met. It should not be possible for objects constructed within a factory function to be used while uninitialized.
 * [ ] Consider forcing `ErrorCallback` to throw.
 * [ ] Pass std::string instead of C-style strings for clarity.
 * [ ] Create a debug mode for compilation.
 * [ ] Add more compile flags (look up GCC, Clang, and MSVC flags).
 * [ ] Use CppLint.
 * [ ] Add concurrency tests and use Clang's Thread Sanitizer (TSAN) to check for data races.
 * [ ] Remove use of `reinterpret_cast` when loading a SPIR-V shader (if possible).
 * [ ] ~~Ensure thread-safety for GLFW functions.~~ Unable to thouroughly test thread-safety without testing functionality on MacOS. Meanwhile, add tests for GLFW features in multithreaded environments. Such tests may take a long time, so an "advanced testing mode" may need to be added to the build system.
 * [ ] Render textures with Vulkan.
 * [ ] Add Freetype for rendering text.
 * [ ] Further compartmentalize Vulkan functions and permit more customization at runtime.
 * [ ] Replace Vulkan-Hpp with Vulkan to improve compiletime (?) (possibly problematic due to a lack of enums and RAII in C-Vulkan).

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
