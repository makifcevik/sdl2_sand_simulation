#include <gtest/gtest.h>

// A dummy test to prove GTest works
TEST(SanityCheck, BasicMath) {
  EXPECT_EQ(2 + 2, 4);
}

// A failing test to see what happens
TEST(SanityCheck, IntentionalFail) {
  EXPECT_NE(1 + 1, 3);
}