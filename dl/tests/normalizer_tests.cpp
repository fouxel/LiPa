#include <gtest/gtest.h>
#include <normalizer.h>

TEST(NormalizerTests, correct1) {
  distvec vec = { 10, 20, 30, 40 };
  distvec output = Normalizer::normalize(vec);
  ASSERT_EQ(1, output[0]);
  ASSERT_EQ(2, output[1]);
  ASSERT_EQ(3, output[2]);
  ASSERT_EQ(4, output[3]);
}