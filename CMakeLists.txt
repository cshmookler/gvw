cmake_minimum_required(VERSION 3.22.1)
project(vulkan_template)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -D _DEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall")
set(CMAKE_EXE_LINKER_FLAGS "-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
    vulkan_template
    src/vgw/glfw/glfw.cpp
    src/vgw/glfw/window.cpp
    src/utils.cpp
    src/main.cpp
)

target_link_libraries(vulkan_template ${CONAN_LIBS})