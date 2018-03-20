find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(..)

add_executable(sim_tests
  normalizer_tests.cpp
  main.cpp)

target_link_libraries(sim_tests ${GTEST_LIBRARIES} pthread sim)

add_test(NAME SIM_TESTS
         COMMAND ./sim_tests
)