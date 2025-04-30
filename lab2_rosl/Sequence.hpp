#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "Option.hpp"
#include <iostream>

template<typename T>
class Sequence {
public:
    virtual T GetFirst()    const = 0;
    virtual T GetLast()     const = 0;
    virtual T Get(int)      const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* GetSubsequence(int, int) const = 0;

    virtual Sequence<T>* Append(T)    = 0;
    virtual Sequence<T>* Prepend(T)   = 0;
    virtual Sequence<T>* InsertAt(T, int) = 0;
    virtual Sequence<T>* RemoveAt(int)     = 0;
    virtual Sequence<T>* Concat(const Sequence<T>*) const = 0;

    virtual Option<T> TryGet(int) const = 0;
    virtual Option<T> TryFirst()  const = 0;
    virtual Option<T> TryLast()   const = 0;

    virtual ~Sequence() = default;

    bool operator==(const Sequence<T>& o) const {
        if (GetLength() != o.GetLength()) return false;
        for (int i = 0; i < GetLength(); ++i)
            if (Get(i) != o.Get(i)) return false;
        return true;
    }
    bool operator!=(const Sequence<T>& o) const { return !(*this == o); }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Sequence<T>& seq) {
    os << "[";
    int n = seq.GetLength();
    for (int i = 0; i < n; ++i) {
        os << seq.Get(i);
        if (i + 1 < n) os << ", ";
    }
    os << "]";
    return os;
}

#endif 
