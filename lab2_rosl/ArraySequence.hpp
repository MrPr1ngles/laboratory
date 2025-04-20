#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "Option.hpp"
#include <stdexcept>

template <typename T> class MutableArraySequence;
template <typename T> class ImmutableArraySequence;

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;
public:
    ArraySequence(T* itemsArray, int count) {
        items = new DynamicArray<T>(itemsArray, count);
    }
    ArraySequence(const DynamicArray<T>& dynamicArray) {
        items = new DynamicArray<T>(dynamicArray);
    }
    ArraySequence(const ArraySequence<T>& other) {
        items = new DynamicArray<T>(*other.items);
    }
    virtual ~ArraySequence() {
        delete items;
    }

    virtual T GetFirst() const override {
        if (items->GetSize() == 0)
            throw std::out_of_range("Index out of range");
        return items->Get(0);
    }

    virtual T GetLast() const override {
        int sz = items->GetSize();
        if (sz == 0)
            throw std::out_of_range("Index out of range");
        return items->Get(sz - 1);
    }

    virtual T Get(int index) const override {
        return items->Get(index);
    }

    virtual int GetLength() const override {
        return items->GetSize();
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        int sz = items->GetSize();
        if (startIndex < 0 || endIndex >= sz || startIndex > endIndex)
            throw std::out_of_range("Index out of range");

        int newCount = endIndex - startIndex + 1;
        T* subItems = new T[newCount];
        for (int i = 0; i < newCount; ++i) {
            subItems[i] = items->Get(startIndex + i);
        }
        Sequence<T>* seq = new MutableArraySequence<T>(subItems, newCount);
        delete[] subItems;
        return seq;
    }

    virtual Option<T> TryGet(int index) const override {
        if (index < 0 || index >= items->GetSize())
            return Option<T>::None();
        return Option<T>::Some(items->Get(index));
    }

    virtual Option<T> TryFirst() const override {
        if (items->GetSize() == 0)
            return Option<T>::None();
        return Option<T>::Some(items->Get(0));
    }

    virtual Option<T> TryLast() const override {
        int sz = items->GetSize();
        if (sz == 0)
            return Option<T>::None();
        return Option<T>::Some(items->Get(sz - 1));
    }

    virtual Sequence<T>* Append(T item) override = 0;
    virtual Sequence<T>* Prepend(T item) override = 0;
    virtual Sequence<T>* InsertAt(T item, int index) override = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* list) const override = 0;
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence(T* itemsArray, int count)
      : ArraySequence<T>(itemsArray, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other)
      : ArraySequence<T>(other) {}

    virtual Sequence<T>* Append(T item) override {
        int sz = this->items->GetSize();
        this->items->Resize(sz + 1);
        this->items->Set(sz, item);
        return this;
    }

    virtual Sequence<T>* Prepend(T item) override {
        int sz = this->items->GetSize();
        this->items->Resize(sz + 1);
        for (int i = sz; i > 0; --i)
            this->items->Set(i, this->items->Get(i - 1));
        this->items->Set(0, item);
        return this;
    }

    virtual Sequence<T>* InsertAt(T item, int index) override {
        int sz = this->items->GetSize();
        if (index < 0 || index > sz)
            throw std::out_of_range("Index out of range");
        this->items->Resize(sz + 1);
        for (int i = sz; i > index; --i)
            this->items->Set(i, this->items->Get(i - 1));
        this->items->Set(index, item);
        return this;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* list) const override {
        int len1 = this->items->GetSize();
        int len2 = list->GetLength();
        this->items->Resize(len1 + len2);
        for (int i = 0; i < len2; ++i)
            this->items->Set(len1 + i, list->Get(i));
        return const_cast<MutableArraySequence<T>*>(this);
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence(T* itemsArray, int count)
      : ArraySequence<T>(itemsArray, count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
      : ArraySequence<T>(other) {}

    virtual Sequence<T>* Append(T item) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        int sz = clone->items->GetSize();
        clone->items->Resize(sz + 1);
        clone->items->Set(sz, item);
        return clone;
    }

    virtual Sequence<T>* Prepend(T item) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        int sz = clone->items->GetSize();
        clone->items->Resize(sz + 1);
        for (int i = sz; i > 0; --i)
            clone->items->Set(i, clone->items->Get(i - 1));
        clone->items->Set(0, item);
        return clone;
    }

    virtual Sequence<T>* InsertAt(T item, int index) override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        int sz = clone->items->GetSize();
        if (index < 0 || index > sz)
            throw std::out_of_range("Index out of range");
        clone->items->Resize(sz + 1);
        for (int i = sz; i > index; --i)
            clone->items->Set(i, clone->items->Get(i - 1));
        clone->items->Set(index, item);
        return clone;
    }

    virtual Sequence<T>* Concat(const Sequence<T>* list) const override {
        auto* clone = new ImmutableArraySequence<T>(*this);
        int sz = clone->items->GetSize();
        int len2 = list->GetLength();
        clone->items->Resize(sz + len2);
        for (int i = 0; i < len2; ++i)
            clone->items->Set(sz + i, list->Get(i));
        return clone;
    }
};

#endif
