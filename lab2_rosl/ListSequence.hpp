#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Option.hpp"
#include <stdexcept>

template <typename T> class MutableListSequence;
template <typename T> class ImmutableListSequence;

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;
public:
    ListSequence(T* itemsArray, int count) {
        list = new LinkedList<T>(itemsArray, count);
    }
    ListSequence(const LinkedList<T>& linkedList) {
        list = new LinkedList<T>(linkedList);
    }
    ListSequence(const ListSequence<T>& other) {
        list = new LinkedList<T>(*other.list);
    }
    virtual ~ListSequence() {
        delete list;
    }

    virtual T GetFirst() const override {
        return list->GetFirst();
    }
    virtual T GetLast() const override {
        return list->GetLast();
    }
    virtual T Get(int index) const override {
        return list->Get(index);
    }
    virtual int GetLength() const override {
        return list->GetLength();
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* sub = list->GetSubList(startIndex, endIndex);
        Sequence<T>* seq = new MutableListSequence<T>(*sub);
        delete sub;
        return seq;
    }

    virtual Option<T> TryGet(int index) const override {
        return list->TryGet(index);
    }
    virtual Option<T> TryFirst() const override {
        return list->TryFirst();
    }
    virtual Option<T> TryLast() const override {
        return list->TryLast();
    }

    virtual Sequence<T>* Append(T item) override = 0;
    virtual Sequence<T>* Prepend(T item) override = 0;
    virtual Sequence<T>* InsertAt(T item, int index) override = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override = 0;
};

template <typename T>
class MutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    virtual Sequence<T>* Append(T item) override {
        this->list->Append(item);
        return this;
    }
    virtual Sequence<T>* Prepend(T item) override {
        this->list->Prepend(item);
        return this;
    }
    virtual Sequence<T>* InsertAt(T item, int index) override {
        this->list->InsertAt(item, index);
        return this;
    }
    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override {
        auto* result = new MutableListSequence<T>(*this);
        for (int i = 0; i < seq->GetLength(); ++i) {
            result->list->Append(seq->Get(i));
        }
        return result;
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    virtual Sequence<T>* Append(T item) override {
        auto* clone = new ImmutableListSequence<T>(*this);
        clone->list->Append(item);
        return clone;
    }
    virtual Sequence<T>* Prepend(T item) override {
        auto* clone = new ImmutableListSequence<T>(*this);
        clone->list->Prepend(item);
        return clone;
    }
    virtual Sequence<T>* InsertAt(T item, int index) override {
        auto* clone = new ImmutableListSequence<T>(*this);
        clone->list->InsertAt(item, index);
        return clone;
    }
    virtual Sequence<T>* Concat(const Sequence<T>* seq) const override {
        auto* clone = new ImmutableListSequence<T>(*this);
        for (int i = 0; i < seq->GetLength(); ++i) {
            clone->list->Append(seq->Get(i));
        }
        return clone;
    }
};

#endif