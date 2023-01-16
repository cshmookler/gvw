cmake_minimum_required(VERSION 3.22.1)
project(gvw VERSION 1.0.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -D _DEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall")

option(GVW_BUILD_SHARED_LIBS "build a shared library" OFF)
option(GVW_BUILD_EXAMPLES "build example programs" ON)

# ----------- Build GVW ----------- #
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi")
set(GVW_SOURCE_FILES

    gvw/common/global.cpp
    gvw/common/file.cpp
    gvw/common/types.cpp
    gvw/common/image.cpp

    gvw/glfw/init.cpp
    gvw/glfw/monitor.cpp
    gvw/glfw/input.cpp
    gvw/glfw/window.cpp
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(GVW_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${GVW_SOURCE_FILES})
else()
    add_library(${PROJECT_NAME} STATIC ${GVW_SOURCE_FILES})
endif()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} -lglfw)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/gvw)
# --------------------------------- #

# ---------- Build Tests ---------- #
if(GVW_BUILD_EXAMPLES)
    set(TARGET_NAMES test)

    add_executable(test examples/test/utils.cpp examples/test/main.cpp)

    target_link_directories(test PRIVATE ${CMAKE_BINARY_DIR})
    target_link_libraries(test gvw)

    file(COPY examples/test/icon.png examples/test/icon_no_transparency.png DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()
# --------------------------------- #