cmake_minimum_required(VERSION 3.22.1)
project(gvw VERSION 1.0.0)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++17 -D _DEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++17 -Wall")

option(GVW_BUILD_SHARED_LIBS "build a shared library" OFF)
option(GVW_BUILD_EXAMPLES "build example programs" ON)

# ---------------- Build GVW ---------------- #

# --- GVW Dependencies --- #

# Dynamic and static libraries
set(GVW_DEP_DYNAMIC_LIBS

    -lstdc++fs
    -lcpplocate
)
set(GVW_DEP_STATIC_LIBS

    libstdc++fs.a
    libcpplocate.a
)

# ------------------------ #

# GVW Source Files
set(GVW_SOURCE_FILES

    # common
    gvw/common/global.cpp
    gvw/common/file.cpp
    gvw/common/types.cpp
    gvw/common/image.cpp

    # glfw
    gvw/glfw/init.cpp
    gvw/glfw/monitor.cpp
    gvw/glfw/input.cpp
    gvw/glfw/window.cpp

    # vulkan

)

if(GVW_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${GVW_SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${GVW_DEP_DYNAMIC_LIBS} glfw )
else()
    add_library(${PROJECT_NAME} STATIC ${GVW_SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${GVW_DEP_STATIC_LIBS} glfw)
endif()

# install(TARGETS ${PROJECT_NAME} DESTINATION lib/gvw)
# ------------------------------------------- #

# add_subdirectory(gvw)

if (GVW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# if (GLFW_BUILD_TESTS)
#     add_subdirectory(tests)
# endif()

# if (DOXYGEN_FOUND AND GLFW_BUILD_DOCS)
#     add_subdirectory(docs)
# endif()