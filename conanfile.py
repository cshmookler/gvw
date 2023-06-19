from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd

class gvw(ConanFile):
    # Required
    name = "gvw"
    version = "1.0.0"

    # Metadata
    license = "MIT"
    author = "Caden Shmookler (cshmookler@gmail.com)"
    url = "https://github.com/cshmookler/GLFW-and-Vulkan-Wrapper.git"
    description = "Desktop application development with GLFW and Vulkan"
    topics = ("C++", "GLFW", "Vulkan")

    # Configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "gvw_static": [True, False],
        "gvw_shared": [True, False],
        "gvw_examples": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "gvw_static": True,
        "gvw_shared": True,
        "gvw_examples": True,
        "fPIC": True
    }

    # Sources
    exports_sources = "CMakeLists.txt", "gvw/*", "examples/*"

    def validate(self):
        check_min_cppstd(self, "17")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("glfw/3.3.8")
        self.requires("vulkan-headers/1.3.243.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder="build")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variable["GVW_CONAN"] = True
        tc.variables["GVW_STATIC"] = self.options.gvw_static
        tc.variables["GVW_SHARED"] = self.options.gvw_shared
        tc.variables["GVW_EXAMPLES"] = self.options.gvw_examples
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
