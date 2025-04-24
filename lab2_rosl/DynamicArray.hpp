#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <cstring>
#include "Error.hpp"

template <typename T>
class DynamicArray {
    T* data;
    int capacity;
public:
    DynamicArray(T* items, int count) {
        capacity = count;
        data = new T[capacity];
        std::memcpy(data, items, sizeof(T)*count);
    }
    DynamicArray(int size) {
        capacity = size;
        data = new T[capacity]();
    }
    DynamicArray(const DynamicArray<T>& o) {
        capacity = o.capacity;
        data = new T[capacity];
        std::memcpy(data, o.data, sizeof(T)*capacity);
    }
    ~DynamicArray() {
        delete[] data;
    }

    T Get(int index) const {
        if (index < 0 || index >= capacity)
            throw Errors[INDEX_OUT_OF_RANGE].message;
        return data[index];
    }
    int GetSize() const { return capacity; }

    void Set(int index, T value) {
        if (index < 0 || index >= capacity)
            throw Errors[INDEX_OUT_OF_RANGE].message;
        data[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0)
            throw Errors[INVALID_INPUT].message;
        T* nd = new T[newSize];
        int m = (newSize < capacity ? newSize : capacity);
        for (int i = 0; i < m; ++i)
            nd[i] = data[i];
        for (int i = m; i < newSize; ++i)
            nd[i] = T();
        delete[] data;
        data = nd;
        capacity = newSize;
    }
};

#endif 
