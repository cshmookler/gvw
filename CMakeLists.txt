cmake_minimum_required(VERSION 3.22.1)
project(gvw)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -D _DEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi")
set(GVW_SOURCE_FILES

    gvw/glfw/glfw.cpp
    gvw/glfw/monitor.cpp
    gvw/glfw/window.cpp
    gvw/vulkan/vulkan.cpp
)

option(GVW_BUILD_SHARED_LIBS "build a shared library" OFF)
option(GVW_BUILD_EXAMPLES "build example programs" ON)
option(GVW_INSTALL "install libararies to your system" OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(examples)

if(GVW_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${GVW_SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${GVW_SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} -lglfw)

if(GVW_INSTALL)
    install(TARGETS ${PROJECT_NAME} DESTINATION lib)
endif()