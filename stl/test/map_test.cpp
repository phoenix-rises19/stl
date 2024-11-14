#include <gtest/gtest.h>
#include "vai/map.hpp"

using namespace vai;

TEST(MapTest, InsertAndBalanceCheck) {
    Map<int, int> map;
    map.insert(10, 1);
    map.insert(20, 2);
    map.insert(5, 3);
    map.insert(6, 4);
    map.insert(15, 5);

    EXPECT_EQ(map.isBalance(),true);
    EXPECT_EQ(map.size(),2);

    auto it = map.begin();
    EXPECT_EQ(it->second,3);
     ++it;
     EXPECT_EQ(*it, 4);
      ++it;
      EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 5);
    ++it;
    EXPECT_EQ(*it, 2);
     ++it;
    EXPECT_EQ(it, map.end());
}
