#include <gtest/gtest.h>
#include <vai/vector.hpp>
using namespace vai;
TEST(VectorTest, InitiallyEmpty) {
    vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, AddElementIncreasesSize) {
    vector<int> v;
    v.push_back(42);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}