add_executable(test utils.cpp main.cpp)

target_link_directories(test PRIVATE ${CMAKE_BINARY_DIR})
target_link_libraries(test gvw)

file(COPY icon.png icon_no_transparency.png DESTINATION ${CMAKE_BINARY_DIR}/examples/test)
