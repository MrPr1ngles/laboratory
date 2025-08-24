#ifndef CORE_DYNAMICARRAY_HPP
#define CORE_DYNAMICARRAY_HPP

// Реализация динамического массива без использования std::vector.
// Использует placement new и ::operator new.
// Заменены все size_t на core::index_t и std::move на core::my_move.

#include "Common.hpp"
#include "Error.hpp"
#include <new>

namespace core {

template<typename T>
class DynamicArray {
    T* data_;
    core::index_t size_;
    core::index_t capacity_;

    void ensure_capacity(core::index_t mincap) {
        if (capacity_ >= mincap) return;
        core::index_t newcap = capacity_ ? capacity_ : 1;
        while (newcap < mincap) newcap <<= 1;
        T* newdata = nullptr;
        try {
            newdata = static_cast<T*>(::operator new(sizeof(T) * (size_t)newcap));
        } catch(...) {
            abort_with_error(ErrorCode::ALLOCATION_ERROR, "DynamicArray::ensure_capacity");
        }
        // move-construct elements
        for (core::index_t i = 0; i < size_; ++i) new (&newdata[i]) T(core::my_move(data_[i]));
        // destroy old elements and free memory
        for (core::index_t i = 0; i < size_; ++i) data_[i].~T();
        ::operator delete(static_cast<void*>(data_));
        data_ = newdata;
        capacity_ = newcap;
    }

public:
    DynamicArray(): data_(nullptr), size_(0), capacity_(0) {}
    explicit DynamicArray(core::index_t n): data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            ensure_capacity(n);
            for (core::index_t i=0;i<n;++i) new (&data_[i]) T();
            size_ = n;
        }
    }
    ~DynamicArray() {
        for (core::index_t i=0;i<size_;++i) data_[i].~T();
        ::operator delete(static_cast<void*>(data_));
    }

    core::index_t size() const { return size_; }
    core::index_t capacity() const { return capacity_; }

    T& operator[](core::index_t i) { return data_[i]; }
    const T& operator[](core::index_t i) const { return data_[i]; }

    void push_back(const T& v) {
        ensure_capacity(size_ + 1);
        new (&data_[size_]) T(v);
        ++size_;
    }
    void push_back(T&& v) {
        ensure_capacity(size_ + 1);
        new (&data_[size_]) T(core::my_move(v));
        ++size_;
    }
    void pop_back() {
        if (size_ == 0) abort_with_error(ErrorCode::EMPTY_CONTAINER, "DynamicArray::pop_back");
        --size_;
        data_[size_].~T();
    }

    void insert(core::index_t idx, const T& v) {
        if (idx > size_) abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "DynamicArray::insert");
        ensure_capacity(size_ + 1);
        // move elements right
        for (core::index_t i = size_; i > idx; --i) {
            new (&data_[i]) T(core::my_move(data_[i-1]));
            data_[i-1].~T();
        }
        new (&data_[idx]) T(v);
        ++size_;
    }

    void remove_at(core::index_t idx) {
        if (idx >= size_) abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "DynamicArray::remove_at");
        data_[idx].~T();
        for (core::index_t i = idx; i + 1 < size_; ++i) {
            new (&data_[i]) T(core::my_move(data_[i+1]));
            data_[i+1].~T();
        }
        --size_;
    }
};

} // namespace core

#endif // CORE_DYNAMICARRAY_HPP
