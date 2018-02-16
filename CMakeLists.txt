cmake_minimum_required(VERSION 3.0)
project(lipa)

add_subdirectory(dl)
add_subdirectory(sim)
add_executable(main main.cpp)

target_link_libraries(main sim ai-model libAIToolboxMDP.a)