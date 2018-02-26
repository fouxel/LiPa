cmake_minimum_required(VERSION 3.0)
project(lipa)

enable_testing()

add_subdirectory(dl)
add_subdirectory(controller)
add_executable(main main.cpp)

target_link_libraries(main sim uart_controller ai-model libAIToolboxMDP.a)
