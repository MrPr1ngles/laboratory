#pragma once
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "Option.hpp"
#include <stdexcept>

template<typename T>
class MutableArraySequence : public Sequence<T> {
private:
    DynamicArray<Option<T>> *data;
    int length;
    int capacity;

    void ensureCapacity(int minCap) {
        if (minCap <= capacity) return;
        int newCap = capacity;
        while (newCap < minCap) {
            newCap = newCap > 0 ? newCap * 2 : 1;
        }
        data->Resize(newCap);
        for (int i = capacity; i < newCap; ++i) {
            data->Set(i, Option<T>::None());
        }
        capacity = newCap;
    }

public:
    MutableArraySequence()
        : data(new DynamicArray<Option<T>>(1)), length(0), capacity(1) {
        data->Set(0, Option<T>::None());
    }

    MutableArraySequence(T* items, int count)
        : data(new DynamicArray<Option<T>>(count > 0 ? count : 1)),
          length(count), capacity(count > 0 ? count : 1) {
        for (int i = 0; i < length; ++i) {
            data->Set(i, Option<T>::Some(items[i]));
        }
        for (int i = length; i < capacity; ++i) {
            data->Set(i, Option<T>::None());
        }
    }

    MutableArraySequence(const MutableArraySequence<T>& o)
        : data(new DynamicArray<Option<T>>(o.capacity)),
          length(o.length), capacity(o.capacity) {
        for (int i = 0; i < capacity; ++i)
            data->Set(i, o.data->Get(i));
    }

    ~MutableArraySequence() override {
        delete data;
    }

    T GetFirst() const override {
        if (length == 0) throw std::out_of_range("GetFirst on empty");
        return data->Get(0).Unwrap();
    }

    T GetLast() const override {
        if (length == 0) throw std::out_of_range("GetLast on empty");
        return data->Get(length - 1).Unwrap();
    }

    T Get(int index) const override {
        if (index < 0 || index >= length)
            throw std::out_of_range("Get index out of range");
        return data->Get(index).Unwrap();
    }

    Option<T> TryGet(int index) const override {
        if (index < 0 || index >= length) return Option<T>::None();
        return data->Get(index);
    }

    Option<T> TryFirst() const override {
        return length > 0 ? data->Get(0) : Option<T>::None();
    }

    Option<T> TryLast() const override {
        return length > 0 ? data->Get(length - 1) : Option<T>::None();
    }

    int GetLength() const override {
        return length;
    }

    Sequence<T>* GetSubsequence(int start, int end) const override {
        if (start < 0 || end < start || end >= length)
            throw std::out_of_range("Subsequence indices out of range");
        int newLen = end - start + 1;
        T* tmp = new T[newLen];
        for (int i = 0; i < newLen; ++i) {
            tmp[i] = data->Get(start + i).Unwrap();
        }
        auto* seq = new MutableArraySequence<T>(tmp, newLen);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* Append(T item) override {
        ensureCapacity(length + 1);
        data->Set(length, Option<T>::Some(item));
        ++length;
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        ensureCapacity(length + 1);
        for (int i = length; i > 0; --i)
            data->Set(i, data->Get(i - 1));
        data->Set(0, Option<T>::Some(item));
        ++length;
        return this;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        if (index < 0 || index > length)
            throw std::out_of_range("InsertAt index out of range");
        ensureCapacity(length + 1);
        for (int i = length; i > index; --i)
            data->Set(i, data->Get(i - 1));
        data->Set(index, Option<T>::Some(item));
        ++length;
        return this;
    }

    Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= length)
            throw std::out_of_range("RemoveAt index out of range");
        for (int i = index; i < length - 1; ++i)
            data->Set(i, data->Get(i + 1));
        --length;
        data->Set(length, Option<T>::None());
        return this;
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        int total = length + other->GetLength();
        T* tmp = new T[total];
        for (int i = 0; i < length; ++i)
            tmp[i] = data->Get(i).Unwrap();
        for (int i = 0; i < other->GetLength(); ++i)
            tmp[length + i] = other->Get(i);
        auto* seq = new MutableArraySequence<T>(tmp, total);
        delete[] tmp;
        return seq;
    }
};

template<typename T>
class ImmutableArraySequence : public Sequence<T> {
private:
    DynamicArray<Option<T>> *data;
    int length;
    int capacity;

public:
    ImmutableArraySequence()
        : data(new DynamicArray<Option<T>>(0)), length(0), capacity(0) {}

    ImmutableArraySequence(T* items, int count)
        : data(new DynamicArray<Option<T>>(count)), length(count), capacity(count) {
        for (int i = 0; i < length; ++i)
            data->Set(i, Option<T>::Some(items[i]));
    }

    ImmutableArraySequence(const ImmutableArraySequence<T>& o)
        : data(new DynamicArray<Option<T>>(o.capacity)),
          length(o.length), capacity(o.capacity) {
        for (int i = 0; i < capacity; ++i)
            data->Set(i, o.data->Get(i));
    }

    ~ImmutableArraySequence() override {
        delete data;
    }

    T GetFirst() const override {
        if (length == 0) throw std::out_of_range("GetFirst on empty");
        return data->Get(0).Unwrap();
    }

    T GetLast() const override {
        if (length == 0) throw std::out_of_range("GetLast on empty");
        return data->Get(length - 1).Unwrap();
    }

    T Get(int index) const override {
        if (index < 0 || index >= length)
            throw std::out_of_range("Get index out of range");
        return data->Get(index).Unwrap();
    }

    Option<T> TryGet(int index) const override {
        if (index < 0 || index >= length) return Option<T>::None();
        return data->Get(index);
    }

    Option<T> TryFirst() const override {
        return length > 0 ? data->Get(0) : Option<T>::None();
    }

    Option<T> TryLast() const override {
        return length > 0 ? data->Get(length - 1) : Option<T>::None();
    }

    int GetLength() const override {
        return length;
    }

    Sequence<T>* GetSubsequence(int start, int end) const override {
        if (start < 0 || end < start || end >= length)
            throw std::out_of_range("Subsequence indices out of range");
        int newLen = end - start + 1;
        T* tmp = new T[newLen];
        for (int i = 0; i < newLen; ++i)
            tmp[i] = data->Get(start + i).Unwrap();
        auto* seq = new ImmutableArraySequence<T>(tmp, newLen);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* Append(T item) override {
        T* tmp = new T[length + 1];
        for (int i = 0; i < length; ++i)
            tmp[i] = data->Get(i).Unwrap();
        tmp[length] = item;
        auto* seq = new ImmutableArraySequence<T>(tmp, length + 1);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* Prepend(T item) override {
        T* tmp = new T[length + 1];
        tmp[0] = item;
        for (int i = 0; i < length; ++i)
            tmp[i + 1] = data->Get(i).Unwrap();
        auto* seq = new ImmutableArraySequence<T>(tmp, length + 1);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* InsertAt(T item, int index) override {
        if (index < 0 || index > length)
            throw std::out_of_range("InsertAt index out of range");
        T* tmp = new T[length + 1];
        for (int i = 0; i < index; ++i)
            tmp[i] = data->Get(i).Unwrap();
        tmp[index] = item;
        for (int i = index; i < length; ++i)
            tmp[i + 1] = data->Get(i).Unwrap();
        auto* seq = new ImmutableArraySequence<T>(tmp, length + 1);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= length)
            throw std::out_of_range("RemoveAt index out of range");
        T* tmp = new T[length - 1];
        for (int i = 0, j = 0; i < length; ++i) {
            if (i == index) continue;
            tmp[j++] = data->Get(i).Unwrap();
        }
        auto* seq = new ImmutableArraySequence<T>(tmp, length - 1);
        delete[] tmp;
        return seq;
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        int total = length + other->GetLength();
        T* tmp = new T[total];
        for (int i = 0; i < length; ++i)
            tmp[i] = data->Get(i).Unwrap();
        for (int i = 0; i < other->GetLength(); ++i)
            tmp[length + i] = other->Get(i);
        auto* seq = new ImmutableArraySequence<T>(tmp, total);
        delete[] tmp;
        return seq;
    }
};
