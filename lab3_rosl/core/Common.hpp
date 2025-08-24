#ifndef CORE_COMMON_HPP
#define CORE_COMMON_HPP

// Общие типы/утилиты проекта (замена <cstddef>, <utility>)

#include <type_traits>

namespace core {

// Заменяем <cstddef>::size_t своим типом индекса.
// На x86_64 обычно unsigned long == size_t. Можно поменять при необходимости.
using index_t = unsigned long;

// Простая замена std::move
template<typename T>
constexpr T&& my_move(T& x) noexcept {
    return static_cast<T&&>(x);
}

} // namespace core

#endif // CORE_COMMON_HPP
