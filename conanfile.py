from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd

def boolToCMake(bool):
    return "ON" if bool else "OFF"

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
        "gvw_warning_as_error": [True, False],
        "gvw_vulkan_validation_layers": [True, False],
        "gvw_static": [True, False],
        "gvw_shared": [True, False],
        "gvw_tests": [True, False],
        "gvw_examples": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "gvw_warning_as_error": False,
        "gvw_vulkan_validation_layers": False,
        "gvw_static": False,
        "gvw_shared": True,
        "gvw_tests": True,
        "gvw_examples": True,
        "fPIC": True
    }

    # Sources
    exports_sources = "CMakeLists.txt", "gvw/*", "examples/*"

    def validate(self):
        check_min_cppstd(self, "20")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("vulkan-loader/1.3.239.0")
        if self.options.gvw_vulkan_validation_layers:
            self.requires("vulkan-validationlayers/1.3.239.0")
        self.requires("glfw/3.3.8")
        self.requires("glm/cci.20230113")
        self.requires("boost/1.82.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.22.6")

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder="build")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(cli_args = [
            "-D GVW_CONAN=ON",
            "-D GVW_WARNING_AS_ERROR=" + boolToCMake(self.options.gvw_warning_as_error),
            "-D GVW_VULKAN_VALIDATION_LAYERS=" + boolToCMake(self.options.gvw_vulkan_validation_layers),
            "-D GVW_STATIC=" + boolToCMake(self.options.gvw_static),
            "-D GVW_SHARED=" + boolToCMake(self.options.gvw_shared),
            "-D GVW_TESTS=" + boolToCMake(self.options.gvw_tests),
            "-D GVW_EXAMPLES=" + boolToCMake(self.options.gvw_examples)
        ])
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def package_info(self):
        self.cpp_info.libs = ["gvw"]
