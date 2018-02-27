
#include "statecoder.h"
#include <gtest/gtest.h>
#include <cmath>
#include <iostream>
 
TEST(StateCoderTests, duplicates) {
    int maxValue = 5;
    int stateCount = std::pow(maxValue + 1, 5);
    std::vector<int> testData[stateCount];
    int count = 0;
    for (int a = 0; a <= maxValue; ++a) {
      for (int b = 0; b <= maxValue; ++b) {
        for (int c = 0; c <= maxValue; ++c) {
          for (int d = 0; d <= maxValue; ++d) {
            for (int e = 0; e <= maxValue; ++e) {
              testData[count++] = {a, b, c, d, e};
            }
          }
        }
      }
    }

    ASSERT_EQ(stateCount, count);
    std::vector<int> states;
    for(int i = 0; i < count; ++i) {
      states.push_back(StateCoder::encode(testData[i], 5));
    }
    
    //Look for duplicates
    for (auto &stateOuter : states) {
      int c = 0;
      for (auto &stateInner : states) {
        if (stateInner == stateOuter) {
          c++;
        }
      }
      ASSERT_EQ(1, c);
    }
}
