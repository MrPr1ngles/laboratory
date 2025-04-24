#ifndef ARRAYSEQUENCE_HPP
#define ARRAYSEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "Error.hpp"

template <typename T> class MutableArraySequence;
template <typename T> class ImmutableArraySequence;

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;
public:
    ArraySequence(T* arr, int count)      { items = new DynamicArray<T>(arr, count); }
    ArraySequence(const ArraySequence<T>& o){ items = new DynamicArray<T>(*o.items); }
    ~ArraySequence() override             { delete items; }

    T GetFirst() const override {
        if (items->GetSize() == 0) throw Errors[INDEX_OUT_OF_RANGE].message;
        return items->Get(0);
    }
    T GetLast() const override {
        int s = items->GetSize();
        if (s == 0) throw Errors[INDEX_OUT_OF_RANGE].message;
        return items->Get(s-1);
    }
    T Get(int i) const override { return items->Get(i); }
    int GetLength() const override { return items->GetSize(); }

    Sequence<T>* GetSubsequence(int a, int b) const override {
        int s = items->GetSize();
        if (a<0||b>=s||a>b) throw Errors[INDEX_OUT_OF_RANGE].message;
        int len = b - a + 1;
        T* tmp = new T[len];
        for (int i = 0; i < len; ++i)
            tmp[i] = items->Get(a + i);
        Sequence<T>* seq = new MutableArraySequence<T>(tmp, len);
        delete[] tmp;
        return seq;
    }

    Option<T> TryGet(int i) const override {
        if (i<0||i>=items->GetSize()) return Option<T>::None();
        return Option<T>::Some(items->Get(i));
    }
    Option<T> TryFirst() const override {
        if (items->GetSize()==0) return Option<T>::None();
        return Option<T>::Some(items->Get(0));
    }
    Option<T> TryLast() const override {
        int s = items->GetSize();
        if (s==0) return Option<T>::None();
        return Option<T>::Some(items->Get(s-1));
    }

    Sequence<T>* RemoveAt(int idx) override {
        int s = items->GetSize();
        if (idx < 0 || idx >= s) throw Errors[INDEX_OUT_OF_RANGE].message;
        for (int i = idx; i + 1 < s; ++i)
            items->Set(i, items->Get(i+1));
        items->Resize(s-1);
        return this;
    }
    
    Sequence<T>* Append(T) override = 0;
    Sequence<T>* Prepend(T) override = 0;
    Sequence<T>* InsertAt(T,int) override = 0;
    Sequence<T>* Concat(const Sequence<T>*) const override = 0;
};

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence(T* arr,int count) : ArraySequence<T>(arr,count) {}
    MutableArraySequence(const MutableArraySequence<T>& o) : ArraySequence<T>(o) {}

    Sequence<T>* Append(T v) override {
        int s = this->items->GetSize();
        this->items->Resize(s+1);
        this->items->Set(s, v);
        return this;
    }
    Sequence<T>* Prepend(T v) override {
        int s = this->items->GetSize();
        this->items->Resize(s+1);
        for(int i=s;i>0;--i) this->items->Set(i,this->items->Get(i-1));
        this->items->Set(0,v);
        return this;
    }
    Sequence<T>* InsertAt(T v,int idx) override {
        int s = this->items->GetSize();
        if (idx<0||idx> s) throw Errors[INDEX_OUT_OF_RANGE].message;
        this->items->Resize(s+1);
        for(int i=s;i>idx;--i) this->items->Set(i,this->items->Get(i-1));
        this->items->Set(idx,v);
        return this;
    }
    Sequence<T>* Concat(const Sequence<T>* o) const override {
        int s1 = this->items->GetSize(), s2 = o->GetLength();
        this->items->Resize(s1+s2);
        for(int i=0;i<s2;++i) this->items->Set(s1+i,o->Get(i));
        return const_cast<MutableArraySequence<T>*>(this);
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence(T* arr,int count) : ArraySequence<T>(arr,count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& o)
      : ArraySequence<T>(o) {}

    Sequence<T>* Append(T v) override {
        auto* c = new ImmutableArraySequence<T>(*this);
        int s = c->items->GetSize();
        c->items->Resize(s+1);
        c->items->Set(s,v);
        return c;
    }
    Sequence<T>* Prepend(T v) override {
        auto* c = new ImmutableArraySequence<T>(*this);
        int s = c->items->GetSize();
        c->items->Resize(s+1);
        for(int i=s;i>0;--i) c->items->Set(i,c->items->Get(i-1));
        c->items->Set(0,v);
        return c;
    }
    Sequence<T>* InsertAt(T v,int idx) override {
        auto* c = new ImmutableArraySequence<T>(*this);
        int s = c->items->GetSize();
        if (idx<0||idx> s) throw Errors[INDEX_OUT_OF_RANGE].message;
        c->items->Resize(s+1);
        for(int i=s;i>idx;--i) c->items->Set(i,c->items->Get(i-1));
        c->items->Set(idx,v);
        return c;
    }
    Sequence<T>* Concat(const Sequence<T>* o) const override {
        auto* c = new ImmutableArraySequence<T>(*this);
        int s1 = c->items->GetSize(), s2 = o->GetLength();
        c->items->Resize(s1+s2);
        for(int i=0;i<s2;++i) c->items->Set(s1+i,o->Get(i));
        return c;
    }
};

#endif 
