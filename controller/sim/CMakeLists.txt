cmake_minimum_required(VERSION 3.0)
project(sim_controller)

set(CMAKE_AUTOMOC ON)

include_directories(../../ai)
find_package(Qt5Widgets REQUIRED)

add_library(sim_controller STATIC
	    line.h
	    line.cpp
	    sim.h
	    sim.cpp
	    world.h
	    world.cpp
	    normalizer.cpp
	    normalizer.h
	    counter.cpp
	    counter.h
)

target_link_libraries(sim_controller Qt5::Widgets)
add_subdirectory(tests)