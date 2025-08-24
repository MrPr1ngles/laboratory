#ifndef UTILS_SORTING_HPP
#define UTILS_SORTING_HPP

// Простая реализация стабильной сортировки merge sort для массивоподобных
// последовательностей: мы реализуем функцию, принимающую указатели на буфер,
// либо контейнер с operator[] и size().
// Реализуем как шаблон для T[].

#include "../core/Common.hpp"
#include "../core/Error.hpp"
#include <new>

namespace utils {

// Сортирует массив data[0..n-1] с использованием оператора "<" или компаратора cmp(a,b) -> bool.
template<typename T, typename Less>
void merge_sort_inplace(T* data, core::index_t n, Less cmp) {
    if (n < 2) return;
    core::index_t mid = n / 2;
    merge_sort_inplace(data, mid, cmp);
    merge_sort_inplace(data + mid, n - mid, cmp);

    // merge two sorted halves into temp
    T* temp = nullptr;
    try {
        temp = static_cast<T*>(::operator new(sizeof(T) * (size_t)n));
    } catch(...) {
        core::abort_with_error(core::ErrorCode::ALLOCATION_ERROR, "merge_sort_inplace");
    }

    core::index_t i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        if (cmp(data[i], data[j])) {
            new (&temp[k++]) T(core::my_move(data[i++]));
        } else {
            new (&temp[k++]) T(core::my_move(data[j++]));
        }
    }
    while (i < mid) new (&temp[k++]) T(core::my_move(data[i++]));
    while (j < n) new (&temp[k++]) T(core::my_move(data[j++]));

    // move back
    for (core::index_t t=0;t<n;++t) {
        // destroy original
        data[t].~T();
    }
    for (core::index_t t=0;t<n;++t) {
        new (&data[t]) T(core::my_move(temp[t]));
    }
    // destruct temp buffer elements and free
    for (core::index_t t=0;t<n;++t) temp[t].~T();
    ::operator delete(static_cast<void*>(temp));
}

template<typename T>
void merge_sort_inplace_default(T* data, core::index_t n) {
    merge_sort_inplace<T>(data, n, [](const T& a, const T& b){ return a < b; });
}

} // namespace utils

#endif // UTILS_SORTING_HPP
