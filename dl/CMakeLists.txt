cmake_minimum_required (VERSION 3.0)
project(ai-lipamodel)

find_package(Boost 1.53 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_library(ai-model STATIC
	    imodel.h
            qlearningmodel.h
	    qlearningmodel.cpp
	   )


add_executable(testMain main.cpp)

target_link_libraries(testMain ai-model libAIToolboxMDP.a Qt5::Widgets)
