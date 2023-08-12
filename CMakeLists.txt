message("-- CMake version: ${CMAKE_VERSION}")
cmake_minimum_required(VERSION 3.22.6)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GVW_PROJECT_NAME gvw)
project(${GVW_PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Prevent in-source builds
get_filename_component(GVW_SOURCE_DIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(GVW_BINARY_DIR "${CMAKE_BINARY_DIR}" REALPATH)
if ("${GVW_SOURCE_DIR}" STREQUAL "${GVW_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed. Delete \"CMakeCache.txt\" and run CMake from another directory.")
endif()

# Compile options
option(GVW_WARNING_AS_ERROR "build with warnings treated as errors" OFF)
option(GVW_STATIC "build as a static library" OFF)
option(GVW_SHARED "build as a shared/dynamic library" ON)
option(GVW_TESTS "build test programs" ON)
option(GVW_EXAMPLES "build example programs" ON)

# Set compilation flags
if(MSVC)
    add_compile_options(/W4)
    if(GVW_WARNING_AS_ERROR)
        add_compile_options(/WX)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(GVW_WARNING_AS_ERROR)
        add_compile_options(-Werror)
    endif()
endif()

# Export compile commands for clang tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find dependencies
find_package(Vulkan REQUIRED)
# find_package(Vulkan COMPONENTS glslang REQUIRED)
find_package(glslang REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(glm REQUIRED)
set(GVW_DEPENDENCIES
    ${Vulkan_LIBRARIES}
    # Vulkan::glslang
    glslang::glslang
    glslang::SPIRV
    glslang::glslang-default-resource-limits
    Boost::filesystem
    glm::glm)
if (GVW_CONAN)
    find_package(glfw3 REQUIRED)
    set(GVW_DEPENDENCIES ${GVW_DEPENDENCIES} ${glfw3_LIBRARIES})
else()
    set(GVW_DEPENDENCIES ${GVW_DEPENDENCIES} glfw)
endif()

# GVW source files
set(GVW_SOURCE_FILES
    gvw/gvw.cpp
    gvw/monitor.cpp
    gvw/window.cpp
    gvw/device.cpp)

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

# Build GVW tests
if(GVW_TESTS)
    if("${GVW_AVAILABLE}" STREQUAL "")
        message(FATAL_ERROR
            "Cannot build tests without also building GVW as either a"
            "static or shared library. Add \"-D GVW_STATIC=ON\" and/or"
            "\"-D GVW_SHARED=ON\" to your CMake command.")
    endif()

    # set(CMAKE_CXX_STANDARD 20)
    # set(CMAKE_CXX_STANDARD_REQUIRED ON)
    # add_executable(threads tests/threads/main.cpp)
    # target_link_libraries(threads PRIVATE ${GVW_AVAILABLE})
    # set_target_properties(threads PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/threads")
    # add_custom_command(TARGET threads POST_BUILD COMMAND $<TARGET_FILE:threads>)
endif()

# Build GVW examples
if(GVW_EXAMPLES)
    if("${GVW_AVAILABLE}" STREQUAL "")
        message(FATAL_ERROR
            "Cannot build examples without also building GVW as either a"
            "static or shared library. Add \"-D GVW_STATIC=ON\" and/or"
            "\"-D GVW_SHARED=ON\" to your CMake command.")
    endif()

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_executable(screen_freeze examples/screen_freeze/main.cpp)
    target_link_libraries(screen_freeze PRIVATE ${GVW_AVAILABLE})
    set_target_properties(screen_freeze PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/screen_freeze")

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_executable(minimal examples/minimal/main.cpp)
    target_link_libraries(minimal PRIVATE ${GVW_AVAILABLE})
    set_target_properties(minimal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/minimal")
    configure_file(examples/minimal/vert.spv "${CMAKE_BINARY_DIR}/examples/minimal/vert.spv" COPYONLY)
    configure_file(examples/minimal/frag.spv "${CMAKE_BINARY_DIR}/examples/minimal/frag.spv" COPYONLY)
endif()