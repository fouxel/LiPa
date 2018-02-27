cmake_minimum_required (VERSION 3.0)
project(ai-lipamodel-tests)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(..)

add_executable(dl_tests
  statecoder_tests.cpp
  normalizer_tests.cpp
  main.cpp)

target_link_libraries(dl_tests ${GTEST_LIBRARIES} pthread ai-model)

add_test(NAME DL_TESTS
         COMMAND ./dl_tests
)