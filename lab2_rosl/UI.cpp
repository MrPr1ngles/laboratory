#include <iostream>
#include <cstring>
#include "UI.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Sequence.hpp"
#include "Error.hpp"

static const int MAX_SEQ = 10;
static Sequence<int>*    intSeqs[MAX_SEQ] = {};
static int               intCount         = 0;
static Sequence<double>* dblSeqs[MAX_SEQ] = {};
static int               dblCount         = 0;

Sequence<int>* createIntSequence(bool useArray) {
    int mode; 
    std::cout << "1 Mutable  2 Immutable: "; 
    std::cin >> mode;
    bool mut = (mode == 1);

    std::cout << "Count: "; 
    int n; 
    std::cin >> n;
    if (n < 0) throw Errors[INVALID_INPUT].message;

    int* arr = new int[n];
    for (int i = 0; i < n; ++i) std::cin >> arr[i];

    Sequence<int>* seq = nullptr;
    if (mut) {
        seq = useArray
            ? static_cast<Sequence<int>*>(new MutableArraySequence<int>(arr, n))
            : static_cast<Sequence<int>*>(new MutableListSequence<int>(arr, n));
    } else {
        seq = useArray
            ? static_cast<Sequence<int>*>(new ImmutableArraySequence<int>(arr, n))
            : static_cast<Sequence<int>*>(new ImmutableListSequence<int>(arr, n));
    }
    delete[] arr;
    return seq;
}

void displayIntSequence(const Sequence<int>* seq) {
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); ++i)
        std::cout << seq->Get(i) << " ";
    std::cout << "]\n";
}

void intOperationsMenu(Sequence<int>*& seq) {
    while (true) {
        std::cout << "1 Display  2 Append  3 Prepend  4 Insert  5 Concat  "
                     "6 Get  7 Subseq  8 Remove  0 Back: ";
        int c; 
        std::cin >> c;
        if (c == 0) break;
        try {
            if (c == 1) {
                displayIntSequence(seq);
            } else if (c == 2) {
                int v; std::cin >> v;
                auto* ns = seq->Append(v);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 3) {
                int v; std::cin >> v;
                auto* ns = seq->Prepend(v);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 4) {
                int v, idx; std::cin >> v >> idx;
                auto* ns = seq->InsertAt(v, idx);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 5) {
                Sequence<int>* other = createIntSequence(true);
                auto* ns = seq->Concat(other);
                delete other;
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 6) {
                int idx; std::cin >> idx;
                std::cout << seq->Get(idx) << "\n";
            } else if (c == 7) {
                int a, b; std::cin >> a >> b;
                auto* sub = seq->GetSubsequence(a, b);
                displayIntSequence(sub);
                delete sub;
            } else if (c == 8) {
                int idx; std::cin >> idx;
                seq->RemoveAt(idx);
            } else {
                std::cout << "Invalid command\n";
            }
        } catch (const char* err) {
            int code = (std::strcmp(err, "Index out of range") == 0)
                       ? INDEX_OUT_OF_RANGE
                       : INVALID_INPUT;
            std::cout << "Error " << code << ": "
                      << GetErrorMessage(code) << "\n";
        }
    }
}

Sequence<double>* createDoubleSequence(bool useArray) {
    int mode; 
    std::cout << "1 Mutable  2 Immutable: "; 
    std::cin >> mode;
    bool mut = (mode == 1);

    std::cout << "Count: "; 
    int n; 
    std::cin >> n;
    if (n < 0) throw Errors[INVALID_INPUT].message;

    double* arr = new double[n];
    for (int i = 0; i < n; ++i) std::cin >> arr[i];

    Sequence<double>* seq = nullptr;
    if (mut) {
        seq = useArray
            ? static_cast<Sequence<double>*>(new MutableArraySequence<double>(arr, n))
            : static_cast<Sequence<double>*>(new MutableListSequence<double>(arr, n));
    } else {
        seq = useArray
            ? static_cast<Sequence<double>*>(new ImmutableArraySequence<double>(arr, n))
            : static_cast<Sequence<double>*>(new ImmutableListSequence<double>(arr, n));
    }
    delete[] arr;
    return seq;
}

void displayDoubleSequence(const Sequence<double>* seq) {
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); ++i)
        std::cout << seq->Get(i) << " ";
    std::cout << "]\n";
}

void doubleOperationsMenu(Sequence<double>*& seq) {
    while (true) {
        std::cout << "1 Display  2 Append  3 Prepend  4 Insert  5 Concat  "
                     "6 Get  7 Subseq  8 Remove  0 Back: ";
        int c; 
        std::cin >> c;
        if (c == 0) break;
        try {
            if (c == 1) {
                displayDoubleSequence(seq);
            } else if (c == 2) {
                double v; std::cin >> v;
                auto* ns = seq->Append(v);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 3) {
                double v; std::cin >> v;
                auto* ns = seq->Prepend(v);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 4) {
                double v; int idx; std::cin >> v >> idx;
                auto* ns = seq->InsertAt(v, idx);
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 5) {
                Sequence<double>* other = createDoubleSequence(true);
                auto* ns = seq->Concat(other);
                delete other;
                if (ns != seq) { delete seq; seq = ns; }
            } else if (c == 6) {
                int idx; std::cin >> idx;
                std::cout << seq->Get(idx) << "\n";
            } else if (c == 7) {
                int a, b; std::cin >> a >> b;
                auto* sub = seq->GetSubsequence(a, b);
                displayDoubleSequence(sub);
                delete sub;
            } else if (c == 8) {
                int idx; std::cin >> idx;
                seq->RemoveAt(idx);
            } else {
                std::cout << "Invalid command\n";
            }
        } catch (const char* err) {
            int code = (std::strcmp(err, "Index out of range") == 0)
                       ? INDEX_OUT_OF_RANGE
                       : INVALID_INPUT;
            std::cout << "Error " << code << ": "
                      << GetErrorMessage(code) << "\n";
        }
    }
}

static void showMainMenu() {
    std::cout << "1 int  2 double  0 exit: ";
}

void UI::run() {
    while (true) {
        showMainMenu();
        int m;
        if (!(std::cin >> m)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input\n";
            continue;
        }
        if (m == 0) break;
        else if (m == 1) {
            std::cout << "1 Array  2 List  3 ListAll  4 Select  0 Back: ";
            int s; std::cin >> s;
            if (s == 1 && intCount < MAX_SEQ) {
                intSeqs[intCount++] = createIntSequence(true);
            } else if (s == 2 && intCount < MAX_SEQ) {
                intSeqs[intCount++] = createIntSequence(false);
            } else if (s == 3) {
                for (int i = 0; i < intCount; ++i) {
                    std::cout << i << ": ";
                    displayIntSequence(intSeqs[i]);
                }
            } else if (s == 4) {
                int idx; std::cin >> idx;
                if (idx >= 0 && idx < intCount)
                    intOperationsMenu(intSeqs[idx]);
                else
                    std::cout << "Invalid index\n";
            } else {
                std::cout << "Invalid option\n";
            }
        } else if (m == 2) {
            std::cout << "1 Array  2 List  3 ListAll  4 Select  0 Back: ";
            int s; std::cin >> s;
            if (s == 1 && dblCount < MAX_SEQ) {
                dblSeqs[dblCount++] = createDoubleSequence(true);
            } else if (s == 2 && dblCount < MAX_SEQ) {
                dblSeqs[dblCount++] = createDoubleSequence(false);
            } else if (s == 3) {
                for (int i = 0; i < dblCount; ++i) {
                    std::cout << i << ": ";
                    displayDoubleSequence(dblSeqs[i]);
                }
            } else if (s == 4) {
                int idx; std::cin >> idx;
                if (idx >= 0 && idx < dblCount)
                    doubleOperationsMenu(dblSeqs[idx]);
                else
                    std::cout << "Invalid index\n";
            } else {
                std::cout << "Invalid option\n";
            }
        } else {
            std::cout << "Invalid menu choice\n";
        }
    }

    for (int i = 0; i < intCount; ++i)
        delete intSeqs[i];
    for (int i = 0; i < dblCount; ++i)
        delete dblSeqs[i];
}
