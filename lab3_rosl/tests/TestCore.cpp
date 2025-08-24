// Примеры тестов (используют gtest).
// Заготовка: добавьте реальные тесты, ориентируясь на требования.
// Здесь — несколько базовых проверок для ArraySequence и DynamicArray.

#include <gtest/gtest.h>
#include "../core/ArraySequence.hpp"
#include "../core/DynamicArray.hpp"

TEST(CoreBasic, DynamicArrayPushPop) {
    core::DynamicArray<int> da;
    da.push_back(1);
    da.push_back(2);
    da.push_back(3);
    ASSERT_EQ(da.size(), 3u);
    da.pop_back();
    ASSERT_EQ(da.size(), 2u);
    ASSERT_EQ(da[0], 1);
    ASSERT_EQ(da[1], 2);
}

TEST(ArraySequence, BasicOps) {
    core::ArraySequence<int> s;
    s.append(10);
    s.append(20);
    s.insert(1, 15);
    ASSERT_EQ(s.size(), 3u);
    ASSERT_EQ(s.get(0), 10);
    ASSERT_EQ(s.get(1), 15);
    ASSERT_EQ(s.get(2), 20);

    auto mapped = s.map([](const int& v, core::index_t idx){ return v + (int)idx; });
    ASSERT_EQ(mapped->size(), 3u);
    delete mapped;
}
