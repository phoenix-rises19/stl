#include <gtest/gtest.h>
#include "vai/map.hpp"

using namespace vai;

TEST(MapTest, InsertAndIteratorCheck) {
    Map<int, int> map;

    map.insert(15,1);
    map.insert(12,1);
    map.insert(18,1);
    map.insert(8,1);
    map.insert(14,1);
    EXPECT_EQ(map.treeHeight(),3);
    EXPECT_EQ(map.getRoot(),15);
    map.insert(9,1);
    EXPECT_EQ(map.treeHeight(),3);
    EXPECT_EQ(map.getRoot(),12);

    map.erase(12);
    ASSERT_EQ(map.treeHeight(), 3);
    ASSERT_EQ(map.getRoot(), 14);
}
