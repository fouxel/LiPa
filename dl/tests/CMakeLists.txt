find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
include_directories(..)

add_executable(ai_model_tests
  statecoder_tests.cpp
  main.cpp)

target_link_libraries(ai_model_tests ${GTEST_LIBRARIES} pthread ai_model)

add_test(NAME AI_MODEL_TESTS
         COMMAND ./ai_model_tests
)