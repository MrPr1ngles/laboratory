#ifndef LISTSEQUENCE_HPP
#define LISTSEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Error.hpp"
#include "Floyd.hpp"

template<typename T> class MutableListSequence;
template<typename T> class ImmutableListSequence;

template<typename T>
class ListSequence : public Sequence<T> {
public:
    std::pair<bool,T> FindCycle() const { return findCycle(*this->list); }
protected:
    LinkedList<T>* list;
public:
    ListSequence(T* arr,int c){ this->list=new LinkedList<T>(arr,c); }
    ListSequence(const LinkedList<T>& o){ this->list=new LinkedList<T>(o); }
    ListSequence(const ListSequence<T>& o){ this->list=new LinkedList<T>(*o.list); }
    ~ListSequence() override{ delete this->list; }
    T GetFirst()    const override{ return this->list->GetFirst(); }
    T GetLast()     const override{ return this->list->GetLast(); }
    T Get(int i)    const override{ return this->list->Get(i); }
    int GetLength() const override{ return this->list->GetLength(); }
    Sequence<T>* GetSubsequence(int a,int b) const override{
        auto* sub=this->list->GetSubList(a,b);
        auto* seq=new MutableListSequence<T>(*sub);
        delete sub;
        return seq;
    }
    Option<T> TryGet(int i)   const override{ return this->list->TryGet(i); }
    Option<T> TryFirst()      const override{ return this->list->TryFirst(); }
    Option<T> TryLast()       const override{ return this->list->TryLast(); }
    Sequence<T>* RemoveAt(int idx) override{
        int L=this->list->GetLength();
        if(idx<0||idx>=L) throw Errors[INDEX_OUT_OF_RANGE].message;
        auto* newl=new LinkedList<T>();
        for(int i=0;i<L;++i) if(i!=idx) newl->Append(this->list->Get(i));
        delete this->list;
        this->list=newl;
        return this;
    }
    Sequence<T>* Append(T)    override=0;
    Sequence<T>* Prepend(T)   override=0;
    Sequence<T>* InsertAt(T,int) override=0;
    Sequence<T>* Concat(const Sequence<T>*) const override=0;
};

template<typename T>
class MutableListSequence: public ListSequence<T> {
public:
    MutableListSequence(T* arr,int c):ListSequence<T>(arr,c){}
    MutableListSequence(const LinkedList<T>& o):ListSequence<T>(o){}
    MutableListSequence(const MutableListSequence<T>& o):ListSequence<T>(o){}
    Sequence<T>* Append(T v) override{ this->list->Append(v); return this; }
    Sequence<T>* Prepend(T v) override{ this->list->Prepend(v); return this; }
    Sequence<T>* InsertAt(T v,int i) override{ this->list->InsertAt(v,i); return this; }
    Sequence<T>* Concat(const Sequence<T>* o) const override{
        for(int i=0;i<o->GetLength();++i) this->list->Append(o->Get(i));
        return const_cast<MutableListSequence<T>*>(this);
    }
    void CreateCycle(int idx){ this->list->CreateCycle(idx); }
};

template<typename T>
class ImmutableListSequence: public ListSequence<T> {
public:
    ImmutableListSequence(T* arr,int c):ListSequence<T>(arr,c){}
    ImmutableListSequence(const LinkedList<T>& o):ListSequence<T>(o){}
    ImmutableListSequence(const ImmutableListSequence<T>& o):ListSequence<T>(o){}
    Sequence<T>* Append(T v) override{
        auto* c=new ImmutableListSequence<T>(*this);
        c->list->Append(v);
        return c;
    }
    Sequence<T>* Prepend(T v) override{
        auto* c=new ImmutableListSequence<T>(*this);
        c->list->Prepend(v);
        return c;
    }
    Sequence<T>* InsertAt(T v,int i) override{
        auto* c=new ImmutableListSequence<T>(*this);
        c->list->InsertAt(v,i);
        return c;
    }
    Sequence<T>* Concat(const Sequence<T>* o) const override{
        auto* c=new ImmutableListSequence<T>(*this);
        for(int i=0;i<o->GetLength();++i) c->list->Append(o->Get(i));
        return c;
    }
};

#endif 
