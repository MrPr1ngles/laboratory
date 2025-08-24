// ui/ui.cpp
// Короткий контроллер UI — меню, использует реестр типов.

#include <iostream>
#include <limits>

#include "../core/DynamicArray.hpp"
#include "../core/Common.hpp"
#include "../core/Error.hpp"

// Локальные ui заголовки (в той же директории)
#include "ADTDefs.hpp"
#include "type_registry.hpp"
#include "type_io.hpp"

using namespace std;
using namespace core;
using namespace ui;

int main_ui();

// helper: read int safe
static bool read_int_ui(int &out) {
    return ui::read_int_safe(out);
}

int main_ui() {
    DynamicArray<ADTEntry> store;
    cout << "Lab3 UI (registry-based). Create and operate on ADTs.\n";

    while (true) {
        cout << "\nMain menu:\n";
        cout << "1) Create new ADT\n";
        cout << "2) List ADTs\n";
        cout << "3) Select ADT by index and operate\n";
        cout << "0) Exit\n";
        cout << "Choice: ";
        int cmd; if (!read_int_ui(cmd)) { cout << "Invalid\n"; continue; }
        if (cmd == 0) break;

        if (cmd == 1) {
            cout << "Choose ADT: 1) Stack  2) SegmentedDeque : ";
            int a; while (!read_int_ui(a) || (a!=1 && a!=2)) cout<<"Invalid choice: ";
            ADTKind adt = (a==1) ? ADTKind::STACK : ADTKind::DEQUE;

            cout << "Choose data type:\n";
            cout << "1) int  2) double  3) complex  4) cstring  5) student  6) teacher\nChoice: ";
            int t; while (!read_int_ui(t) || t<1 || t> (int)g_type_registry_count) cout<<"Invalid: ";
            std::size_t type_idx = static_cast<std::size_t>(t-1);

            void* obj = nullptr;
            if (adt == ADTKind::STACK) obj = g_type_registry[type_idx].create_stack();
            else obj = g_type_registry[type_idx].create_deque(16);

            ADTEntry e;
            e.adt = adt;
            e.dt  = static_cast<DataKind>(t);
            e.obj = obj;
            store.push_back(e);
            cout << "Created and stored at index " << (store.size() - 1) << "\n";
            continue;
        }

        if (cmd == 2) {
            cout << "Current ADTs:\n";
            for (index_t i = 0; i < store.size(); ++i) {
                cout << i << ": ";
                ADTEntry &e = store[i];
                if (!e.obj) { cout << "<empty>\n"; continue; }
                cout << ((e.adt == ADTKind::STACK) ? "Stack " : "Deque ");
                switch (e.dt) {
                    case DataKind::INT: cout << "int"; break;
                    case DataKind::DOUBLE: cout << "double"; break;
                    case DataKind::COMPLEX: cout << "complex"; break;
                    case DataKind::CSTRING: cout << "cstring"; break;
                    case DataKind::STUDENT: cout << "student"; break;
                    case DataKind::TEACHER: cout << "teacher"; break;
                    default: cout << "unknown"; break;
                }
                cout << " (size: ";
                if (e.adt == ADTKind::STACK) {
                    switch (e.dt) {
                        case DataKind::INT: cout << static_cast<Stack<int>*>(e.obj)->size(); break;
                        case DataKind::DOUBLE: cout << static_cast<Stack<double>*>(e.obj)->size(); break;
                        case DataKind::COMPLEX: cout << static_cast<Stack<types::Complex>*>(e.obj)->size(); break;
                        case DataKind::CSTRING: cout << static_cast<Stack<types::CString>*>(e.obj)->size(); break;
                        case DataKind::STUDENT: cout << static_cast<Stack<types::Student>*>(e.obj)->size(); break;
                        case DataKind::TEACHER: cout << static_cast<Stack<types::Teacher>*>(e.obj)->size(); break;
                        default: cout << "?"; break;
                    }
                } else {
                    switch (e.dt) {
                        case DataKind::INT: cout << static_cast<SegmentedDeque<int>*>(e.obj)->size(); break;
                        case DataKind::DOUBLE: cout << static_cast<SegmentedDeque<double>*>(e.obj)->size(); break;
                        case DataKind::COMPLEX: cout << static_cast<SegmentedDeque<types::Complex>*>(e.obj)->size(); break;
                        case DataKind::CSTRING: cout << static_cast<SegmentedDeque<types::CString>*>(e.obj)->size(); break;
                        case DataKind::STUDENT: cout << static_cast<SegmentedDeque<types::Student>*>(e.obj)->size(); break;
                        case DataKind::TEACHER: cout << static_cast<SegmentedDeque<types::Teacher>*>(e.obj)->size(); break;
                        default: cout << "?"; break;
                    }
                }
                cout << ")\n";
            }
            continue;
        }

        if (cmd == 3) {
            cout << "Enter ADT index: ";
            index_t idx;
            if (!ui::read_index(idx) || idx >= store.size()) { cout << "Invalid index\n"; continue; }
            ADTEntry &entry = store[idx];
            if (!entry.obj) { cout << "Empty entry\n"; continue; }
            std::size_t type_idx = static_cast<std::size_t>((int)entry.dt - 1);
            if (type_idx >= g_type_registry_count) { cout << "Invalid type\n"; continue; }
            g_type_registry[type_idx].interactive_ops(static_cast<void*>(&entry), store, idx);
            continue;
        }

        cout << "Unknown command\n";
    }

    // cleanup: delete all entries
    for (index_t i = 0; i < store.size(); ++i) {
        ADTEntry &e = store[i];
        if (!e.obj) continue;
        std::size_t type_idx = static_cast<std::size_t>((int)e.dt - 1);
        if (type_idx >= g_type_registry_count) continue;
        if (e.adt == ADTKind::STACK) g_type_registry[type_idx].delete_stack(e.obj);
        else g_type_registry[type_idx].delete_deque(e.obj);
        e.obj = nullptr;
    }

    cout << "Exiting UI.\n";
    return 0;
}
