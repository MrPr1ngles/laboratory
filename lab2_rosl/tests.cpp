#include <cassert>
#include <iostream>
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Option.hpp"
#include "Error.hpp"

int main() {
    int iarr[] = {1,2,3,4,5};
    const int n = 5;

    // 1) MutableArraySequence<int>
    {
        Sequence<int>* seq = new MutableArraySequence<int>(iarr, n);
        assert(seq->GetLength() == n);
        assert(seq->GetFirst() == 1);
        assert(seq->GetLast()  == 5);
        for (int i = 0; i < n; ++i)
            assert(seq->Get(i) == iarr[i]);
        assert(seq->TryGet(2).IsSome() && seq->TryGet(2).Unwrap() == 3);
        assert(seq->TryGet(n).IsNone());
        assert(seq->TryFirst().Unwrap() == 1);
        assert(seq->TryLast().Unwrap()  == 5);

        Sequence<int>* sub = seq->GetSubsequence(1, 3);
        assert(sub->GetLength() == 3);
        assert(sub->Get(0) == 2 && sub->Get(2) == 4);
        delete sub;

        seq->Append(6);           // length = 6
        seq->Prepend(0);          // length = 7
        seq->InsertAt(100, 2);    // length = 8

        int carr[] = {7,8};
        Sequence<int>* other = new MutableArraySequence<int>(carr, 2);
        seq->Concat(other);       // length = 10
        delete other;

        Sequence<int>* remArr = seq->RemoveAt(1);
        assert(remArr == seq);
        assert(seq->GetLength() == n + 4);
        assert(seq->Get(1) == 100);

        delete seq;
    }

    // 2) MutableListSequence<int>
    {
        Sequence<int>* seq = new MutableListSequence<int>(iarr, n);
        assert(seq->GetLength() == n);
        assert(seq->GetFirst() == 1);
        assert(seq->GetLast()  == 5);

        seq->Append(6);           // length = 6
        seq->Prepend(0);          // length = 7
        seq->InsertAt(100, 2);    // length = 8

        int carr[] = {7,8};
        Sequence<int>* other = new MutableListSequence<int>(carr, 2);
        seq->Concat(other);       // length = 10
        delete other;

        Sequence<int>* remLst = seq->RemoveAt(1);
        assert(remLst == seq);
        assert(seq->GetLength() == n + 4);
        assert(seq->Get(1) == 100);

        delete seq;
    }

    // 3) ImmutableArraySequence<int>
    {
        Sequence<int>* seq = new ImmutableArraySequence<int>(iarr, n);
        Sequence<int>* app = seq->Append(6);
       assert(app != seq);
        assert(seq->GetLength() == n);
        assert(app->GetLength() == n + 1);
        assert(app->Get(5) == 6);
        delete seq;
        delete app;
    }

    // 4) ImmutableListSequence<int>
    {
        Sequence<int>* seq = new ImmutableListSequence<int>(iarr, n);
        Sequence<int>* prep = seq->Prepend(0);
        assert(prep != seq);
        assert(prep->GetLength() == n + 1);
        assert(prep->Get(0) == 0);
        delete seq;
        delete prep;
    }

    // 5) operator==
    {
        auto* a1 = new MutableArraySequence<int>(iarr, n);
        auto* a2 = new MutableArraySequence<int>(iarr, n);
        assert(*a1 == *a2);
        a2->Append(6);
        assert(*a1 != *a2);
        delete a1;
        delete a2;
    }

    std::cout << "All tests passed successfully.\n";
    return 0;
}
