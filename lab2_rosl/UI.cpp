#include <iostream>
#include <cstring>
#include "UI.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Sequence.hpp"
#include "Error.hpp"

static const int MAX_SEQ = 10;
static Sequence<int>*    intSeqs[MAX_SEQ]    = {};
static int               intCount            = 0;
static Sequence<double>* dblSeqs[MAX_SEQ]    = {};
static int               dblCount            = 0;

Sequence<int>*    createIntSequence(bool useArray);
void              displayIntSequence(const Sequence<int>* seq);
void              intOperationsMenu(Sequence<int>* &seq);

Sequence<double>* createDoubleSequence(bool useArray);
void              displayDoubleSequence(const Sequence<double>* seq);
void              doubleOperationsMenu(Sequence<double>* &seq);

static void showMainMenu() {
    std::cout << "\n=== Main Menu ===\n"
              << "1: Manage int sequences\n"
              << "2: Manage double sequences\n"
              << "0: Exit\n"
              << "Choice: ";
}

Sequence<int>* createIntSequence(bool useArray) {
    std::cout << "Choose implementation:\n"
              << " 1: Mutable\n"
              << " 2: Immutable\n"
              << "Choice: ";
    int mode; std::cin >> mode;
    bool mutableMode = (mode == 1);

    std::cout << "Enter number of elements: ";
    int n; std::cin >> n;
    if (n < 0) throw std::invalid_argument("Invalid input");
    int* arr = new int[n];
    std::cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    Sequence<int>* seq = nullptr;
    if (useArray) {
        if (mutableMode)
            seq = new MutableArraySequence<int>(arr, n);
        else
            seq = new ImmutableArraySequence<int>(arr, n);
    } else {
        if (mutableMode)
            seq = new MutableListSequence<int>(arr, n);
        else
            seq = new ImmutableListSequence<int>(arr, n);
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

void intOperationsMenu(Sequence<int>* &seq) {
    while (true) {
        std::cout << "\nOperations:\n"
                  << " 1: Display\n"
                  << " 2: Append\n"
                  << " 3: Prepend\n"
                  << " 4: InsertAt\n"
                  << " 5: Concat (into same mode)\n"
                  << " 6: Get\n"
                  << " 7: Subsequence\n"
                  << " 0: Back\n"
                  << "Choice: ";
        int choice; std::cin >> choice;
        if (choice == 0) break;

        try {
            switch (choice) {
                case 1:
                    displayIntSequence(seq);
                    break;
                case 2: {
                    std::cout << "Value to append: ";
                    int v; std::cin >> v;
                    auto* ns = seq->Append(v);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 3: {
                    std::cout << "Value to prepend: ";
                    int v; std::cin >> v;
                    auto* ns = seq->Prepend(v);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 4: {
                    std::cout << "Value and index: ";
                    int v, idx; std::cin >> v >> idx;
                    auto* ns = seq->InsertAt(v, idx);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 5: {
                    std::cout << "Choose type to concat:\n"
                              << " 1: ArraySequence\n"
                              << " 2: ListSequence\n"
                              << "Choice: ";
                    int t; std::cin >> t;
                    Sequence<int>* tmp = createIntSequence(t==1);
                    auto* ns = seq->Concat(tmp);
                    delete tmp;
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 6: {
                    std::cout << "Index: ";
                    int idx; std::cin >> idx;
                    std::cout << "Value: " << seq->Get(idx) << "\n";
                    break;
                }
                case 7: {
                    std::cout << "Start and end: ";
                    int a,b; std::cin >> a >> b;
                    auto* sub = seq->GetSubsequence(a, b);
                    displayIntSequence(sub);
                    delete sub;
                    break;
                }
                default:
                    std::cout << "Invalid\n";
            }
        }
        catch (const std::exception &e) {
            int code = -1;
            if (!std::strcmp(e.what(), "Index out of range"))
                code = (int)ErrorCode::INDEX_OUT_OF_RANGE;
            else if (!std::strcmp(e.what(), "Invalid input"))
                code = (int)ErrorCode::INVALID_INPUT;
            std::cout << "Error " << code << ": "
                      << GetErrorMessage(code) << "\n";
        }
    }
}

Sequence<double>* createDoubleSequence(bool useArray) {
    std::cout << "Choose implementation:\n"
              << " 1: Mutable\n"
              << " 2: Immutable\n"
              << "Choice: ";
    int mode; std::cin >> mode;
    bool mutableMode = (mode == 1);

    std::cout << "Enter number of elements: ";
    int n; std::cin >> n;
    if (n < 0) throw std::invalid_argument("Invalid input");
    double* arr = new double[n];
    std::cout << "Enter " << n << " doubles: ";
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    Sequence<double>* seq = nullptr;
    if (useArray) {
        if (mutableMode)
            seq = new MutableArraySequence<double>(arr, n);
        else
            seq = new ImmutableArraySequence<double>(arr, n);
    } else {
        if (mutableMode)
            seq = new MutableListSequence<double>(arr, n);
        else
            seq = new ImmutableListSequence<double>(arr, n);
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

void doubleOperationsMenu(Sequence<double>* &seq) {
    while (true) {
        std::cout << "\nOperations:\n"
                  << " 1: Display\n"
                  << " 2: Append\n"
                  << " 3: Prepend\n"
                  << " 4: InsertAt\n"
                  << " 5: Concat\n"
                  << " 6: Get\n"
                  << " 7: Subsequence\n"
                  << " 0: Back\n"
                  << "Choice: ";
        int choice; std::cin >> choice;
        if (choice == 0) break;

        try {
            switch (choice) {
                case 1:
                    displayDoubleSequence(seq);
                    break;
                case 2: {
                    std::cout << "Value to append: ";
                    double v; std::cin >> v;
                    auto* ns = seq->Append(v);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 3: {
                    std::cout << "Value to prepend: ";
                    double v; std::cin >> v;
                    auto* ns = seq->Prepend(v);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 4: {
                    std::cout << "Value and index: ";
                    double v; int idx; std::cin >> v >> idx;
                    auto* ns = seq->InsertAt(v, idx);
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 5: {
                    std::cout << "Choose type to concat:\n"
                              << " 1: ArraySequence\n"
                              << " 2: ListSequence\n"
                              << "Choice: ";
                    int t; std::cin >> t;
                    Sequence<double>* tmp = createDoubleSequence(t==1);
                    auto* ns = seq->Concat(tmp);
                    delete tmp;
                    if (ns != seq) { delete seq; seq = ns; }
                    break;
                }
                case 6: {
                    std::cout << "Index: ";
                    int idx; std::cin >> idx;
                    std::cout << "Value: " << seq->Get(idx) << "\n";
                    break;
                }
                case 7: {
                    std::cout << "Start and end: ";
                    int a,b; std::cin >> a >> b;
                    auto* sub = seq->GetSubsequence(a,b);
                    displayDoubleSequence(sub);
                    delete sub;
                    break;
                }
                default:
                    std::cout << "Invalid\n";
            }
        }
        catch (const std::exception &e) {
            int code = -1;
            if (!std::strcmp(e.what(), "Index out of range"))
                code = (int)ErrorCode::INDEX_OUT_OF_RANGE;
            else if (!std::strcmp(e.what(), "Invalid input"))
                code = (int)ErrorCode::INVALID_INPUT;
            std::cout << "Error " << code << ": "
                      << GetErrorMessage(code) << "\n";
        }
    }
}

// -----------------------------------------------------------------------------
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

        try {
            if (m == 1) {
                std::cout << "\n-- Int Sequences --\n"
                          << "1: Create ArraySequence\n"
                          << "2: Create ListSequence\n"
                          << "3: List all\n"
                          << "4: Select and operate\n"
                          << "0: Back\n"
                          << "Choice: ";
                int sub; std::cin >> sub;
                if      (sub == 1 && intCount < MAX_SEQ)
                    intSeqs[intCount++] = createIntSequence(true);
                else if (sub == 2 && intCount < MAX_SEQ)
                    intSeqs[intCount++] = createIntSequence(false);
                else if (sub == 3) {
                    for (int i = 0; i < intCount; ++i) {
                        std::cout << i << ": ";
                        displayIntSequence(intSeqs[i]);
                    }
                }
                else if (sub == 4) {
                    std::cout << "Index: ";
                    int idx; std::cin >> idx;
                    if (idx>=0 && idx<intCount)
                        intOperationsMenu(intSeqs[idx]);
                    else
                        std::cout<<"Invalid index\n";
                } else std::cout<<"Invalid choice\n";
            }
            else if (m == 2) {
                std::cout << "\n-- Double Sequences --\n"
                          << "1: Create ArraySequence\n"
                          << "2: Create ListSequence\n"
                          << "3: List all\n"
                          << "4: Select and operate\n"
                          << "0: Back\n"
                          << "Choice: ";
                int sub; std::cin >> sub;
                if      (sub == 1 && dblCount < MAX_SEQ)
                    dblSeqs[dblCount++] = createDoubleSequence(true);
                else if (sub == 2 && dblCount < MAX_SEQ)
                    dblSeqs[dblCount++] = createDoubleSequence(false);
                else if (sub == 3) {
                    for (int i = 0; i < dblCount; ++i) {
                        std::cout << i << ": ";
                        displayDoubleSequence(dblSeqs[i]);
                    }
                }
                else if (sub == 4) {
                    std::cout << "Index: ";
                    int idx; std::cin >> idx;
                    if (idx>=0 && idx<dblCount)
                        doubleOperationsMenu(dblSeqs[idx]);
                    else
                        std::cout<<"Invalid index\n";
                } else std::cout<<"Invalid choice\n";
            }
            else {
                std::cout << "Invalid choice\n";
            }
        }
        catch (const std::exception &e) {
            int code = -1;
            if (!std::strcmp(e.what(), "Index out of range"))
                code = (int)ErrorCode::INDEX_OUT_OF_RANGE;
            else if (!std::strcmp(e.what(), "Invalid input"))
                code = (int)ErrorCode::INVALID_INPUT;
            std::cout << "Error " << code << ": " << GetErrorMessage(code) << "\n";
        }
    }

    for(int i=0;i<intCount;++i) delete intSeqs[i];
    for(int i=0;i<dblCount;++i) delete dblSeqs[i];
}
