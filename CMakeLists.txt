cmake_minimum_required(VERSION 3.22.6)
set(CMAKE_CXX_STANDARD 20)
message("-- CMake version: ${CMAKE_VERSION}")

set(GVW_PROJECT_NAME gvw)
project(${GVW_PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Prevent in-source builds
get_filename_component(GVW_SOURCE_DIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(GVW_BINARY_DIR "${CMAKE_BINARY_DIR}" REALPATH)
if ("${GVW_SOURCE_DIR}" STREQUAL "${GVW_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed. Delete \"CMakeCache.txt\" and run CMake from another directory.")
endif()

# Set compilation flags
if(MSVC)
    add_compile_options(/W4 /WX)
    # add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    # add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Export compile commands for clang tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile options
option(GVW_STATIC "build as a static library" OFF)
option(GVW_SHARED "build as a shared/dynamic library" ON)
option(GVW_EXAMPLES "build example programs" ON)

# Find dependencies
if(GVW_CONAN)
    find_package(glfw3 REQUIRED)
    find_package(VulkanHeaders REQUIRED)
    set(GVW_DEPENDENCIES
        glfw
        vulkan-headers::vulkan-headers
    )
else()
    set(GVW_DEPENDENCIES
        glfw
    )
endif()

# GVW source files
set(GVW_SOURCE_FILES
    gvw/types.cpp
    gvw/init.cpp
    gvw/window.cpp
    gvw/monitor.cpp
)

# The name of an available GVW library file.
set(GVW_AVAILABLE)

# Build GVW as a static library
if(GVW_STATIC)
    set(GVW_STATIC_NAME "${GVW_PROJECT_NAME}_static")
    set(GVW_AVAILABLE ${GVW_STATIC_NAME})
    add_library(${GVW_STATIC_NAME} STATIC)
    target_sources(${GVW_STATIC_NAME} PRIVATE ${GVW_SOURCE_FILES})
    target_link_libraries(${GVW_STATIC_NAME} PUBLIC ${GVW_DEPENDENCIES})
    install(TARGETS ${GVW_STATIC_NAME} DESTINATION lib)
endif()

# Build GVW as a shared library
if(GVW_SHARED)
    set(GVW_SHARED_NAME "${GVW_PROJECT_NAME}_shared")
    set(GVW_AVAILABLE ${GVW_SHARED_NAME})
    add_library(${GVW_SHARED_NAME} SHARED)
    target_sources(${GVW_SHARED_NAME} PRIVATE ${GVW_SOURCE_FILES})
    target_link_libraries(${GVW_SHARED_NAME} PUBLIC ${GVW_DEPENDENCIES})
    install(TARGETS ${GVW_SHARED_NAME} DESTINATION lib)
endif()

# Install GVW source files
install(DIRECTORY gvw DESTINATION include)

# Build GVW examples
if(GVW_EXAMPLES)
    if("${GVW_AVAILABLE}" STREQUAL "")
        message(FATAL_ERROR
            "Cannot build examples without also building GVW as either a"
            "static or shared library. Add \"-D GVW_STATIC=ON\" and/or"
            "\"-D GVW_SHARED=ON\" to your CMake command.")
    else()
        add_subdirectory(examples)
    endif()
endif()

# if(GVW_BUILD_TESTS)
#     add_subdirectory(tests)
# endif()

# if(DOXYGEN_FOUND AND GVW_BUILD_DOCS)
#     add_subdirectory(docs)
# endif()
