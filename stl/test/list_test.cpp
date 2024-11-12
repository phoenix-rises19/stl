#include <gtest/gtest.h>
#include "vai/list.hpp"

using namespace vai;

TEST(ListTest, BasicPushBackAndPopBack) {
    List<int> list;
    int val1 = 1;
    int val2 = 2;
    list.push_back(val1);
    list.push_back(val2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.back()->value, 2);
    EXPECT_EQ(list.front()->value, 1);

    list.pop_back();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.back()->value, 1);
}

TEST(ListTest, PushFrontAndPopFront) {
    List<int> list;
    int val1 = 1;
    int val2 = 2;
    list.push_front(val1);
    list.push_front(val2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front()->value, 2);

    list.pop_front();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list.front()->value, 1);
}

TEST(ListTest, ClearList) {
    List<int> list;
    int val1 = 1;
    list.push_back(val1);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, EmplaceBack) {
    List<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front()->value, 1);
    EXPECT_EQ(list.back()->value, 2);
}

TEST(ListTest, IteratorTraversal) {
    List<int> list;
    int val1 = 1, val2 = 2, val3 = 3;
    list.push_back(val1);
    list.push_back(val2);
    list.push_back(val3);
    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ListTest, ReverseList) {
    List<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    list.reverse();
    EXPECT_EQ(list.front()->value, 3);
    EXPECT_EQ(list.back()->value, 1);
}

TEST(ListTest, MergeLists) {
    List<int> list1, list2;
    list1.emplace_back(1);
    list1.emplace_back(3);
    list2.emplace_back(2);
    list2.emplace_back(4);
    list1.merge(list2);
    EXPECT_EQ(list1.size(), 4);
    EXPECT_EQ(list1.front()->value, 1);
    EXPECT_EQ(list1.back()->value, 4);
    EXPECT_TRUE(list2.empty());
}