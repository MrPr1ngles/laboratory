#include <gtest/gtest.h>
#include "../adt/SegmentedDeque.hpp"
#include "../utils/Sorting.hpp"

TEST(SegmentedDequeBasic, PushPop) {
    adt::SegmentedDeque<int> dq(4);
    dq.push_back(10);
    dq.push_back(20);
    ASSERT_EQ(dq.size(), 2u);
    ASSERT_EQ(dq.get(0), 10);
    ASSERT_EQ(dq.get(1), 20);
    dq.pop_back();
    ASSERT_EQ(dq.size(), 1u);
}

TEST(SegmentedDequeFunctional, MapWhereReduce) {
    adt::SegmentedDeque<int> dq(4);
    dq.push_back(1); dq.push_back(2); dq.push_back(3); dq.push_back(4);

    auto doubled = dq.map([](const int& v, core::index_t idx){ return v * 2; });
    ASSERT_EQ(doubled.get(2), 6);

    auto even = dq.where([](const int& v, core::index_t idx){ return (v % 2) == 0; });
    ASSERT_EQ(even.size(), 2u);

    auto sum = dq.reduce([](const int& a, const int& b){ return a + b; });
    ASSERT_TRUE(sum.has_value());
    ASSERT_EQ(sum.value(), 10);
}
