add_executable(block main.cpp)

target_link_directories(block PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(block gvw)
