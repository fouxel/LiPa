cmake_minimum_required(VERSION 3.0)
project(sim)

set(CMAKE_AUTOMOC ON)

include_directories(../../dl)
find_package(Qt5Widgets REQUIRED)

add_library(sim STATIC
	    line.h
	    line.cpp
	    sim.h
	    sim.cpp
	    world.h
	    world.cpp
)

target_link_libraries(sim Qt5::Widgets)