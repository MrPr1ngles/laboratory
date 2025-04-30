#include <cassert>
#include <iostream>
#include <vector>
#include "ArraySequence.hpp"
#include "ListSequence.hpp"

void ADT() {
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

        seq->Append(6);           
        seq->Prepend(0);          
        seq->InsertAt(100, 2);    

        int carr[] = {7,8};
        Sequence<int>* other = new MutableArraySequence<int>(carr, 2);
        seq->Concat(other);      
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

        seq->Append(6);           
        seq->Prepend(0);          
        seq->InsertAt(100, 2);    

        int carr[] = {7,8};
        Sequence<int>* other = new MutableListSequence<int>(carr, 2);
        seq->Concat(other);       
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

    std::cout << "All ATD tests passed successfully.\n";
}

void testFloydEmpty() {
    MutableListSequence<int> seq(nullptr, 0);
    auto pr = seq.FindCycle();
    assert(!pr.first);
}

void testFloydOneNoCycle() {
    int arr[] = {42};
    MutableListSequence<int> seq(arr, 1);
    auto pr = seq.FindCycle();
    assert(!pr.first);
}

void testFloydOneSelfCycle() {
    int arr[] = {7};
    auto* seq = new MutableListSequence<int>(arr, 1);
    seq->CreateCycle(0);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 7);
}

void testFloydTwoNoCycle() {
    int arr[] = {1, 2};
    MutableListSequence<int> seq(arr, 2);
    auto pr = seq.FindCycle();
    assert(!pr.first);
}

void testFloydTwoCycleFirst() {
    int arr[] = {3, 4};
    auto* seq = new MutableListSequence<int>(arr, 2);
    seq->CreateCycle(0);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 3);
}

void testFloydTwoCycleSecond() {
    int arr[] = {5, 6};
    auto* seq = new MutableListSequence<int>(arr, 2);
    seq->CreateCycle(1);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 6);
}

void testFloydMultiCycleAtStart() {
    int arr[] = {10, 20, 30, 40};
    auto* seq = new MutableListSequence<int>(arr, 4);
    seq->CreateCycle(0);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 10);
}

void testFloydMultiCycleMiddle() {
    int arr[] = {11, 22, 33, 44, 55};
    auto* seq = new MutableListSequence<int>(arr, 5);
    seq->CreateCycle(2);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 33);
}

void testFloydMultiCycleAtEnd() {
    int arr[] = {100, 200, 300};
    auto* seq = new MutableListSequence<int>(arr, 3);
    seq->CreateCycle(2);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 300);
}

void testFloydDeepNoCycle() {
    const int N = 10000;
    std::vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i;
    MutableListSequence<int> seq(v.data(), N);
    auto pr = seq.FindCycle();
    assert(!pr.first);
}

void testFloydDeepCycleShallow() {
    const int N = 10000;
    std::vector<int> v(N);
    for (int i = 0; i < N; ++i) v[i] = i;
    auto* seq = new MutableListSequence<int>(v.data(), N);
    seq->CreateCycle(10);
    auto pr = seq->FindCycle();
    assert(pr.first && pr.second == 10);
}

void testInvariantNonCycle() {
    int arr[] = {8, 9, 10};
    MutableListSequence<int> seq(arr, 3);
    std::vector<int> before;
    for (int i = 0; i < seq.GetLength(); ++i) before.push_back(seq.Get(i));
    auto pr = seq.FindCycle();
    assert(!pr.first);
    for (size_t i = 0; i < before.size(); ++i)
        assert(seq.Get(i) == before[i]);
}

int main() {
    ADT();
    testFloydEmpty();
    testFloydOneNoCycle();
    testFloydOneSelfCycle();
    testFloydTwoNoCycle();
    testFloydTwoCycleFirst();
    testFloydTwoCycleSecond();
    testFloydMultiCycleAtStart();
    testFloydMultiCycleMiddle();
    testFloydMultiCycleAtEnd();
    testFloydDeepNoCycle();
    testFloydDeepCycleShallow();
    testInvariantNonCycle();
    std::cout << "All tests passed\n";
    return 0;
}
