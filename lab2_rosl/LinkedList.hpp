#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Option.hpp"
#include "Error.hpp"

template<typename T>
class LinkedList {
public:
    struct Node { T value; Node* next; Node(const T& v): value(v), next(nullptr) {} };
    Node* head;

    LinkedList(): head(nullptr) {}
    LinkedList(T* arr, int count): head(nullptr) { for(int i=0;i<count;++i) Append(arr[i]); }
    LinkedList(const LinkedList<T>& o): head(nullptr) { for(Node* cur=o.head; cur; cur=cur->next) Append(cur->value); }
    ~LinkedList(){ while(head){ Node* t=head; head=head->next; delete t; } }

    void CreateCycle(int idx){
        if(idx<0) throw Errors[INDEX_OUT_OF_RANGE].message;
        Node* target=head;
        for(int i=0;i<idx;++i){
            if(!target) throw Errors[INDEX_OUT_OF_RANGE].message;
            target=target->next;
        }
        if(!target) throw Errors[INDEX_OUT_OF_RANGE].message;
        Node* tail=head;
        if(!tail) return;
        while(tail->next) tail=tail->next;
        tail->next=target;
    }

    T GetFirst() const{ if(!head) throw Errors[INDEX_OUT_OF_RANGE].message; return head->value; }
    T GetLast()  const{ if(!head) throw Errors[INDEX_OUT_OF_RANGE].message; Node* cur=head; while(cur->next) cur=cur->next; return cur->value; }
    T Get(int i) const{ if(i<0) throw Errors[INDEX_OUT_OF_RANGE].message; Node* cur=head; while(cur && i--) cur=cur->next; if(!cur) throw Errors[INDEX_OUT_OF_RANGE].message; return cur->value; }
    LinkedList<T>* GetSubList(int a,int b) const{
        if(a<0||b<a) throw Errors[INDEX_OUT_OF_RANGE].message;
        LinkedList<T>* res=new LinkedList<T>();
        Node* cur=head;
        for(int i=0;i<a;++i){ if(!cur) throw Errors[INDEX_OUT_OF_RANGE].message; cur=cur->next; }
        for(int i=a;i<=b;++i){ if(!cur) throw Errors[INDEX_OUT_OF_RANGE].message; res->Append(cur->value); cur=cur->next; }
        return res;
    }
    int GetLength() const{ int c=0; for(Node* cur=head; cur; cur=cur->next) ++c; return c; }

    void Append(T v){ Node* n=new Node(v); if(!head){ head=n; return; } Node* cur=head; while(cur->next) cur=cur->next; cur->next=n; }
    void Prepend(T v){ Node* n=new Node(v); n->next=head; head=n; }
    void InsertAt(T v,int i){ if(i<0) throw Errors[INDEX_OUT_OF_RANGE].message; if(i==0){ Prepend(v); return; } Node* cur=head; while(cur && --i) cur=cur->next; if(!cur) throw Errors[INDEX_OUT_OF_RANGE].message; Node* n=new Node(v); n->next=cur->next; cur->next=n; }

    Option<T> TryGet(int i) const{ try{ return Option<T>::Some(Get(i)); } catch(...){ return Option<T>::None(); } }
    Option<T> TryFirst() const{ try{ return Option<T>::Some(GetFirst()); } catch(...){ return Option<T>::None(); } }
    Option<T> TryLast()  const{ try{ return Option<T>::Some(GetLast());  } catch(...){ return Option<T>::None(); } }
};

#endif 
