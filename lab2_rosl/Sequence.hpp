#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "Option.hpp"

template <typename T>
class Sequence {
public:
    virtual ~Sequence() {}
    virtual T GetFirst() const = 0;
    virtual T GetLast()  const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<T>* GetSubsequence(int start, int end) const = 0;

    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;

    virtual Option<T> TryGet(int index) const = 0;
    virtual Option<T> TryFirst()    const = 0;
    virtual Option<T> TryLast()     const = 0;
};

#endif 
