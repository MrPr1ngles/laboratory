#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Error.hpp"

template <typename T> class MutableListSequence;
template <typename T> class ImmutableListSequence;

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;
public:
    ListSequence(T* arr, int count) {
        list = new LinkedList<T>(arr, count);
    }
    ListSequence(const LinkedList<T>& linkedList) {
        list = new LinkedList<T>(linkedList);
    }
    ListSequence(const ListSequence<T>& o) {
        list = new LinkedList<T>(*o.list);
    }
    ~ListSequence() override {
        delete list;
    }

    T GetFirst() const override       { return list->GetFirst(); }
    T GetLast()  const override       { return list->GetLast(); }
    T Get(int i) const override       { return list->Get(i); }
    int GetLength() const override    { return list->GetLength(); }

    Sequence<T>* GetSubsequence(int a, int b) const override {
        LinkedList<T>* sub = list->GetSubList(a, b);
        auto* seq = new MutableListSequence<T>(*sub);
        delete sub;
        return seq;
    }

    Option<T> TryGet(int i)   const override { return list->TryGet(i); }
    Option<T> TryFirst()      const override { return list->TryFirst(); }
    Option<T> TryLast()       const override { return list->TryLast(); }

    Sequence<T>* RemoveAt(int idx) override {
        int L = list->GetLength();
        if (idx < 0 || idx >= L) throw Errors[INDEX_OUT_OF_RANGE].message;
        LinkedList<T>* newl = new LinkedList<T>();
        for (int i = 0; i < L; ++i) {
            if (i != idx) newl->Append(list->Get(i));
        }
        delete list;
        list = newl;
        return this;
    }

    Sequence<T>* Append(T) override = 0;
    Sequence<T>* Prepend(T) override = 0;
    Sequence<T>* InsertAt(T, int) override = 0;
    Sequence<T>* Concat(const Sequence<T>*) const override = 0;
};

template <typename T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence(T* arr, int count)
      : ListSequence<T>(arr, count) {}
    MutableListSequence(const LinkedList<T>& linkedList)
      : ListSequence<T>(linkedList) {}
    MutableListSequence(const MutableListSequence<T>& o)
      : ListSequence<T>(o) {}

    Sequence<T>* Append(T v) override {
        this->list->Append(v);
        return this;
    }
    Sequence<T>* Prepend(T v) override {
        this->list->Prepend(v);
        return this;
    }
    Sequence<T>* InsertAt(T v, int idx) override {
        this->list->InsertAt(v, idx);
        return this;
    }
    Sequence<T>* Concat(const Sequence<T>* o) const override {
        for (int i = 0; i < o->GetLength(); ++i) {
            this->list->Append(o->Get(i));
        }
        return const_cast<MutableListSequence<T>*>(this);
    }
};

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence(T* arr, int count)
      : ListSequence<T>(arr, count) {}
    ImmutableListSequence(const LinkedList<T>& linkedList)
      : ListSequence<T>(linkedList) {}
    ImmutableListSequence(const ImmutableListSequence<T>& o)
      : ListSequence<T>(o) {}

    Sequence<T>* Append(T v) override {
        auto* c = new ImmutableListSequence<T>(*this);
        c->list->Append(v);
        return c;
    }
    Sequence<T>* Prepend(T v) override {
        auto* c = new ImmutableListSequence<T>(*this);
        c->list->Prepend(v);
        return c;
    }
    Sequence<T>* InsertAt(T v, int idx) override {
        auto* c = new ImmutableListSequence<T>(*this);
        c->list->InsertAt(v, idx);
        return c;
    }
    Sequence<T>* Concat(const Sequence<T>* o) const override {
        auto* c = new ImmutableListSequence<T>(*this);
        for (int i = 0; i < o->GetLength(); ++i) {
            c->list->Append(o->Get(i));
        }
        return c;
    }
};

#endif 
