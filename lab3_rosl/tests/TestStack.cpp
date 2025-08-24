#include <gtest/gtest.h>
#include "../adt/Stack.hpp"

TEST(StackBasic, PushPopTop) {
    adt::Stack<int> s;
    s.push(1);
    s.push(2);
    ASSERT_EQ(s.top(), 2);
    s.pop();
    ASSERT_EQ(s.top(), 1);
}

TEST(StackMapWhereReduce, Functional) {
    adt::Stack<int> s;
    s.push(1); s.push(2); s.push(3);

    // Используем безымянный параметр для индекса, чтобы убрать предупреждения о неиспользуемом параметре
    auto s2 = s.map([](const int& v, core::index_t){ return v * 2; });
    ASSERT_EQ(s2.top(), 6);

    auto s3 = s2.where([](const int& v, core::index_t){ return (v % 4) == 0; });
    ASSERT_EQ(s3.size(), 1u);

    auto r = s.reduce([](const int& a, const int& b){ return a + b; });
    ASSERT_TRUE(r.has_value());
    ASSERT_EQ(r.value(), 6);
}
