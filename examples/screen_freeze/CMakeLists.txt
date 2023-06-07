add_executable(screen_freeze main.cpp)

target_link_directories(screen_freeze PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(screen_freeze gvw)