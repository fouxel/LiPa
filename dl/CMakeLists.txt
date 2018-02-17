cmake_minimum_required (VERSION 3.0)
project(ai-lipamodel)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_library(ai-model STATIC
	    imodel.h
            qlearningmodel.h
	    qlearningmodel.cpp
	    /usr/local/include/easylogging++.cc
	   )