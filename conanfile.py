import os
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, cmake_layout, CMake

class gvw(ConanFile):
    # Required
    name = "gvw"
    version = "1.0"

    # Metadata
    license = "MIT"
    author = "Caden Shmookler (cshmookler@gmail.com)"
    url = "https://github.com/cshmookler/GLFW-and-Vulkan-Wrapper.git"
    description = "Desktop application development with GLFW and Vulkan"
    topics = ("C++", "GLFW", "Vulkan")

    # Configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources
    exports_sources = "CMakeLists.txt", "gvw/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self, build_folder="build")

    def requirements(self):
        self.requires("glfw/3.3.8")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def package_info(self):
        self.cpp_info.libs = ["gvw"]