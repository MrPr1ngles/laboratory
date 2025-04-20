#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;
public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray();

    T Get(int index) const;
    int GetSize() const;
    void Set(int index, T value);
    void Resize(int newSize);
    T* begin() { return data; }
    T* end() { return data + size; }
    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};

template <typename T>
DynamicArray<T>::DynamicArray(T* items, int count) : size(count) {
    if (count < 0) throw std::invalid_argument("Negative size");
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = items[i];
}

template <typename T>
DynamicArray<T>::DynamicArray(int size) : size(size) {
    if (size < 0) throw std::invalid_argument("Negative size");
    data = new T[size];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : size(other.size) {
    data = new T[size];
    for (int i = 0; i < size; i++)
        data[i] = other.data[i];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

template <typename T>
T DynamicArray<T>::Get(int index) const {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    return data[index];
}

template <typename T>
int DynamicArray<T>::GetSize() const {
    return size;
}

template <typename T>
void DynamicArray<T>::Set(int index, T value) {
    if (index < 0 || index >= size)
        throw std::out_of_range("Index out of range");
    data[index] = value;
}

template <typename T>
void DynamicArray<T>::Resize(int newSize) {
    if (newSize < 0) throw std::invalid_argument("Negative newSize");
    T* newData = new T[newSize];
    int copySize = (newSize < size) ? newSize : size;
    for (int i = 0; i < copySize; i++)
        newData[i] = data[i];
    delete[] data;
    data = newData;
    size = newSize;
}

#endif
