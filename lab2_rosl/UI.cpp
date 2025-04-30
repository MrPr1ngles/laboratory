#include <iostream>
#include <vector>
#include "UI.hpp"
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "Error.hpp"

static void intOperationsMenu(Sequence<int>*& seq, std::vector<Sequence<int>*>& seqs) {
    bool isList = dynamic_cast<ListSequence<int>*>(seq) != nullptr;
    while (true) {
        std::cout << "1)Append 2)Prepend 3)InsertAt 4)RemoveAt 5)Display 6)Get 7)GetFirst 8)GetLast "
                  << "9)GetSubseq 10)Concat 11)TryGet 12)TryFirst 13)TryLast";
        if (isList) std::cout << " 14)FindCycle";
        std::cout << " 0)Back\n> ";
        int cmd; std::cin >> cmd;
        if (cmd == 0) break;
        try {
            if (cmd == 1) { int v; std::cin>>v; seq=seq->Append(v); }
            else if (cmd == 2) { int v; std::cin>>v; seq=seq->Prepend(v); }
            else if (cmd == 3) { int i,v; std::cin>>i>>v; seq=seq->InsertAt(v,i); }
            else if (cmd == 4) { int i; std::cin>>i; seq=seq->RemoveAt(i); }
            else if (cmd == 5) { std::cout<<*seq<<"\n"; }
            else if (cmd == 6) { int i; std::cin>>i; std::cout<<seq->Get(i)<<"\n"; }
            else if (cmd == 7) { std::cout<<seq->GetFirst()<<"\n"; }
            else if (cmd == 8) { std::cout<<seq->GetLast()<<"\n"; }
            else if (cmd == 9) { int a,b; std::cin>>a>>b; auto* s=seq->GetSubsequence(a,b); std::cout<<*s<<"\n"; delete s; }
            else if (cmd == 10) { int idx; std::cin>>idx; if(idx<0||idx>=int(seqs.size())) throw Errors[INDEX_OUT_OF_RANGE].message; seq=seq->Concat(seqs[idx]); }
            else if (cmd == 11) { int i; std::cin>>i; auto o=seq->TryGet(i); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (cmd == 12) { auto o=seq->TryFirst(); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (cmd == 13) { auto o=seq->TryLast(); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (isList && cmd == 14) {
                auto pr = dynamic_cast<ListSequence<int>*>(seq)->FindCycle();
                if (pr.first) std::cout<<"Cycle at "<<pr.second<<"\n"; else std::cout<<"No cycle\n";
            }
        } catch (const char* e) { std::cout<<"Error: "<<e<<"\n"; }
    }
}

static void doubleOperationsMenu(Sequence<double>*& seq, std::vector<Sequence<double>*>& seqs) {
    bool isList = dynamic_cast<ListSequence<double>*>(seq) != nullptr;
    while (true) {
        std::cout << "1)Append 2)Prepend 3)InsertAt 4)RemoveAt 5)Display 6)Get 7)GetFirst 8)GetLast "
                  << "9)GetSubseq 10)Concat 11)TryGet 12)TryFirst 13)TryLast";
        if (isList) std::cout << " 14)FindCycle";
        std::cout << " 0)Back\n> ";
        int cmd; std::cin >> cmd;
        if (cmd == 0) break;
        try {
            if (cmd == 1) { double v; std::cin>>v; seq=seq->Append(v); }
            else if (cmd == 2) { double v; std::cin>>v; seq=seq->Prepend(v); }
            else if (cmd == 3) { int i; double v; std::cin>>i>>v; seq=seq->InsertAt(v,i); }
            else if (cmd == 4) { int i; std::cin>>i; seq=seq->RemoveAt(i); }
            else if (cmd == 5) { std::cout<<*seq<<"\n"; }
            else if (cmd == 6) { int i; std::cin>>i; std::cout<<seq->Get(i)<<"\n"; }
            else if (cmd == 7) { std::cout<<seq->GetFirst()<<"\n"; }
            else if (cmd == 8) { std::cout<<seq->GetLast()<<"\n"; }
            else if (cmd == 9) { int a,b; std::cin>>a>>b; auto* s=seq->GetSubsequence(a,b); std::cout<<*s<<"\n"; delete s; }
            else if (cmd == 10) { int idx; std::cin>>idx; if(idx<0||idx>=int(seqs.size())) throw Errors[INDEX_OUT_OF_RANGE].message; seq=seq->Concat(seqs[idx]); }
            else if (cmd == 11) { int i; std::cin>>i; auto o=seq->TryGet(i); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (cmd == 12) { auto o=seq->TryFirst(); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (cmd == 13) { auto o=seq->TryLast(); if(o.IsSome()) std::cout<<o.Unwrap()<<"\n"; else std::cout<<"None\n"; }
            else if (isList && cmd == 14) {
                auto pr = dynamic_cast<ListSequence<double>*>(seq)->FindCycle();
                if (pr.first) std::cout<<"Cycle at "<<pr.second<<"\n"; else std::cout<<"No cycle\n";
            }
        } catch (const char* e) { std::cout<<"Error: "<<e<<"\n"; }
    }
}

void UI::run() {
    std::vector<Sequence<int>*> intSeqs;
    std::vector<Sequence<double>*> dblSeqs;
    while (true) {
        std::cout << "Type:1)int 2)double 0)Exit\n> ";
        int t; std::cin>>t; if(t==0) break;
        if(t==1) {
            while(true) {
                std::cout<<"1)CreateInt 2)OperateInt 0)Back\n> ";
                int m; std::cin>>m; if(m==0) break;
                if(m==1) {
                    std::cout<<"ADT:1)Array 2)List 0)Cancel\n> ";
                    int a; std::cin>>a; if(a==0) continue;
                    std::cout<<"Mut:1)Mut 2)Imm\n> "; int u; std::cin>>u;
                    std::cout<<"Count:\n> "; int n; std::cin>>n;
                    int* arr = n>0?new int[n]:nullptr;
                    for(int i=0;i<n;++i) std::cin>>arr[i];
                    Sequence<int>* seq=nullptr;
                    if(a==1&&u==1) seq=new MutableArraySequence<int>(arr,n);
                    if(a==1&&u==2) seq=new ImmutableArraySequence<int>(arr,n);
                    if(a==2&&u==1) seq=new MutableListSequence<int>(arr,n);
                    if(a==2&&u==2) seq=new ImmutableListSequence<int>(arr,n);
                    if(a==2&&u==1) {
                        std::cout<<"Create cycle? y/n\n> "; char c; std::cin>>c;
                        if(c=='y') { int idx; std::cout<<"Link tail to index:\n> "; std::cin>>idx; static_cast<MutableListSequence<int>*>(seq)->CreateCycle(idx); }
                    }
                    intSeqs.push_back(seq);
                    delete[] arr;
                    std::cout<<"Index "<<int(intSeqs.size()-1)<<" created\n";
                } else {
                    if(intSeqs.empty()){std::cout<<"None\n";continue;}
                    std::cout<<"Select idx 0.."<<int(intSeqs.size()-1)<<"\n> ";
                    int idx; std::cin>>idx; if(idx<0||idx>=int(intSeqs.size())){std::cout<<"Bad\n";continue;}
                    intOperationsMenu(intSeqs[idx],intSeqs);
                }
            }
        } else {
            while(true) {
                std::cout<<"1)CreateDbl 2)OperateDbl 0)Back\n> ";
                int m; std::cin>>m; if(m==0) break;
                if(m==1) {
                    std::cout<<"ADT:1)Array 2)List 0)Cancel\n> ";
                    int a; std::cin>>a; if(a==0) continue;
                    std::cout<<"Mut:1)Mut 2)Imm\n> "; int u; std::cin>>u;
                    std::cout<<"Count:\n> "; int n; std::cin>>n;
                    double* arr = n>0?new double[n]:nullptr;
                    for(int i=0;i<n;++i) std::cin>>arr[i];
                    Sequence<double>* seq=nullptr;
                    if(a==1&&u==1) seq=new MutableArraySequence<double>(arr,n);
                    if(a==1&&u==2) seq=new ImmutableArraySequence<double>(arr,n);
                    if(a==2&&u==1) seq=new MutableListSequence<double>(arr,n);
                    if(a==2&&u==2) seq=new ImmutableListSequence<double>(arr,n);
                    if(a==2&&u==1) {
                        std::cout<<"Create cycle? y/n\n> "; char c; std::cin>>c;
                        if(c=='y') { int idx; std::cout<<"Link tail to index:\n> "; std::cin>>idx; static_cast<MutableListSequence<double>*>(seq)->CreateCycle(idx); }
                    }
                    dblSeqs.push_back(seq);
                    delete[] arr;
                    std::cout<<"Index "<<int(dblSeqs.size()-1)<<" created\n";
                } else {
                    if(dblSeqs.empty()){std::cout<<"None\n";continue;}
                    std::cout<<"Select idx 0.."<<int(dblSeqs.size()-1)<<"\n> ";
                    int idx; std::cin>>idx; if(idx<0||idx>=int(dblSeqs.size())){std::cout<<"Bad\n";continue;}
                    doubleOperationsMenu(dblSeqs[idx],dblSeqs);
                }
            }
        }
    }
    for(auto*p:intSeqs) delete p;
    for(auto*p:dblSeqs) delete p;
}
