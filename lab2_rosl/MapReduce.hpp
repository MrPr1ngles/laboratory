#ifndef MAPREDUCE_HPP
#define MAPREDUCE_HPP

#include "ArraySequence.hpp"

template <typename T, typename U, typename Func>
Sequence<U>* Map(const Sequence<T>* seq, Func f) {
    int len = seq->GetLength();
    U* arr = new U[len];
    for (int i = 0; i < len; i++) {
        arr[i] = f(seq->Get(i));
    }
    Sequence<U>* result = new MutableArraySequence<U>(arr, len);
    delete[] arr;
    return result;
}

template <typename T, typename Func>
T Reduce(const Sequence<T>* seq, Func f, T init) {
    int len = seq->GetLength();
    T result = init;
    for (int i = 0; i < len; i++) {
        result = f(result, seq->Get(i));
    }
    return result;
}

#endif
