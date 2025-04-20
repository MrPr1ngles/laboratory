#include <cassert>
#include <iostream>
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Option.hpp"

template<typename T>
void testSequence(Sequence<T>* seq, const T* baseArr, int n) {
    assert(seq->GetLength() == n);
    assert(seq->GetFirst() == baseArr[0]);
    assert(seq->GetLast()  == baseArr[n-1]);
    for (int i = 0; i < n; ++i) {
        assert(seq->Get(i) == baseArr[i]);
    }
    auto optIn  = seq->TryGet(2);
    auto optOut = seq->TryGet(n);
    assert(optIn.IsSome() && optIn.Unwrap() == baseArr[2]);
    assert(optOut.IsNone());
    assert(seq->TryFirst().IsSome() && seq->TryFirst().Unwrap() == baseArr[0]);
    assert(seq->TryLast().IsSome()  && seq->TryLast().Unwrap()  == baseArr[n-1]);
    Sequence<T>* sub = seq->GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == baseArr[1]);
    assert(sub->Get(2) == baseArr[3]);
    delete sub;
}

template<typename T>
void testMutable(Sequence<T>* seq, const T* baseArr, int n, bool isArray) {
    auto* app = seq->Append(baseArr[0] + baseArr[n-1]);
    assert(app == seq);
    assert(seq->GetLength() == n + 1);
    assert(seq->Get(n) == baseArr[0] + baseArr[n-1]);
    auto* prep = seq->Prepend(baseArr[0] - baseArr[n-1]);
    assert(prep == seq);
    assert(seq->GetLength() == n + 2);
    assert(seq->Get(0) == baseArr[0] - baseArr[n-1]);
    auto* ins = seq->InsertAt(baseArr[2], 2);
    assert(ins == seq);
    assert(seq->Get(2) == baseArr[2]);
    T tmpArr[] = { 99, 100 };
    Sequence<T>* other = nullptr;
    if (isArray) {
        other = new MutableArraySequence<T>(tmpArr, 2);
    } else {
        other = new MutableListSequence<T>(tmpArr, 2);
    }
    auto* cat = seq->Concat(other);
    if (isArray) {
        assert(cat == seq);
    } else {
        assert(cat != seq);
    }
    assert(cat->GetLength() == n + 5);
    delete other;
    if (!isArray) {
        delete cat;
    }
}

template<typename T>
void testImmutable(const T* baseArr, int n, bool isArray) {
    Sequence<T>* seq = nullptr;
    if (isArray) {
        seq = new ImmutableArraySequence<T>(const_cast<T*>(baseArr), n);
    } else {
        seq = new ImmutableListSequence<T>(const_cast<T*>(baseArr), n);
    }
    auto* app = seq->Append(123);
    assert(app != seq);
    assert(seq->GetLength() == n);
    assert(app->GetLength() == n + 1);
    auto* prep = seq->Prepend(456);
    assert(prep != seq);
    assert(prep->Get(0) == 456);
    auto* ins = seq->InsertAt(789, 1);
    assert(ins != seq);
    assert(ins->Get(1) == 789);
    T tmpArr[] = { 1, 2, 3 };
    Sequence<T>* other = nullptr;
    if (isArray) {
        other = new ImmutableArraySequence<T>(tmpArr, 3);
    } else {
        other = new ImmutableListSequence<T>(tmpArr, 3);
    }
    auto* cat = seq->Concat(other);
    assert(cat != seq);
    assert(cat->GetLength() == n + 3);
    delete seq;
    delete app;
    delete prep;
    delete ins;
    delete cat;
    delete other;
}

int main() {
    int   iarr[] = {1,2,3,4,5};
    double darr[] = {1.1,2.2,3.3,4.4,5.5};
    int n = 5;

    {
        auto* seq = new MutableArraySequence<int>(iarr, n);
        testSequence(seq, iarr, n);
        testMutable(seq, iarr, n, true);
        delete seq;
    }
    {
        auto* seq = new MutableListSequence<int>(iarr, n);
        testSequence(seq, iarr, n);
        testMutable(seq, iarr, n, false);
        delete seq;
    }
    testImmutable<int>(iarr, n, true);
    testImmutable<int>(iarr, n, false);

    {
        auto* seq = new MutableArraySequence<double>(darr, n);
        testSequence(seq, darr, n);
        testMutable(seq, darr, n, true);
        delete seq;
    }
    {
        auto* seq = new MutableListSequence<double>(darr, n);
        testSequence(seq, darr, n);
        testMutable(seq, darr, n, false);
        delete seq;
    }
    testImmutable<double>(darr, n, true);
    testImmutable<double>(darr, n, false);

    std::cout << "All tests passed successfully.\n";
    return 0;
}
