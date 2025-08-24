#ifndef UI_OPS_TEMPLATES_HPP
#define UI_OPS_TEMPLATES_HPP

#include <iostream>
#include <limits>

#include "../core/DynamicArray.hpp"
#include "../core/Common.hpp"
#include "../core/Option.hpp"
#include "../core/Error.hpp"
#include "../adt/Stack.hpp"
#include "../adt/SegmentedDeque.hpp"

#include "ADTDefs.hpp"
#include "type_io.hpp"

namespace ui {

using namespace core;
using namespace adt;

// --- create factories ---
template<typename T>
void* create_stack_from_input_generic() {
    int n;
    std::cout << "Enter number of elements: ";
    while (!read_int_safe(n)) std::cout << "Invalid, enter integer: ";
    Stack<T>* s = new Stack<T>();
    for (int i = 0; i < n; ++i) {
        T v = read_one<T>();
        s->push(v);
    }
    return static_cast<void*>(s);
}

template<typename T>
void* create_deque_from_input_generic(index_t /*segsize*/ = 16) {
    int n;
    std::cout << "Enter number of elements: ";
    while (!read_int_safe(n)) std::cout << "Invalid, enter integer: ";
    SegmentedDeque<T>* d = new SegmentedDeque<T>(16);
    for (int i = 0; i < n; ++i) {
        T v = read_one<T>();
        d->push_back(v);
    }
    return static_cast<void*>(d);
}

// --- print full ---
template<typename T>
void print_stack_full_generic(void* obj) {
    Stack<T>* s = static_cast<Stack<T>*>(obj);
    std::cout << "Stack size: " << s->size() << "\n";
    for (index_t i = 0; i < s->size(); ++i) {
        std::cout << i << ": ";
        print_one<T>(s->at(i));
        std::cout << "\n";
    }
}

template<typename T>
void print_deque_full_generic(void* obj) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    std::cout << "Deque size: " << d->size() << "\n";
    for (index_t i = 0; i < d->size(); ++i) {
        std::cout << i << ": ";
        print_one<T>(d->get(i));
        std::cout << "\n";
    }
}

// --- delete helpers ---
template<typename T>
void delete_stack_t(void* obj) { delete static_cast<Stack<T>*>(obj); }

template<typename T>
void delete_deque_t(void* obj) { delete static_cast<SegmentedDeque<T>*>(obj); }

// --- concat/subsequence/find wrappers returning new allocated void* ---
template<typename T>
void* concat_stack_new(void* a_obj, void* b_obj) {
    Stack<T>* a = static_cast<Stack<T>*>(a_obj);
    Stack<T>* b = static_cast<Stack<T>*>(b_obj);
    Stack<T> res = a->concat(*b);
    return static_cast<void*>( new Stack<T>(my_move(res)) );
}

template<typename T>
void* concat_deque_new(void* a_obj, void* b_obj) {
    SegmentedDeque<T>* a = static_cast<SegmentedDeque<T>*>(a_obj);
    SegmentedDeque<T>* b = static_cast<SegmentedDeque<T>*>(b_obj);
    SegmentedDeque<T> res = a->concat(*b);
    return static_cast<void*>( new SegmentedDeque<T>(my_move(res)) );
}

template<typename T>
void* subseq_stack_new(void* obj, index_t i, index_t j) {
    Stack<T>* s = static_cast<Stack<T>*>(obj);
    Stack<T> res = s->subsequence(i, j);
    return static_cast<void*>( new Stack<T>(my_move(res)) );
}

template<typename T>
void* subseq_deque_new(void* obj, index_t i, index_t j) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    SegmentedDeque<T>* nd = new SegmentedDeque<T>(16);
    for (index_t k = i; k <= j; ++k) nd->push_back(d->get(k));
    return static_cast<void*>(nd);
}

template<typename T>
Option<index_t> find_subseq_stack(void* a_obj, void* b_obj) {
    Stack<T>* a = static_cast<Stack<T>*>(a_obj);
    Stack<T>* b = static_cast<Stack<T>*>(b_obj);
    return a->find_subsequence(*b);
}

template<typename T>
Option<index_t> find_subseq_deque(void* a_obj, void* b_obj) {
    SegmentedDeque<T>* a = static_cast<SegmentedDeque<T>*>(a_obj);
    SegmentedDeque<T>* b = static_cast<SegmentedDeque<T>*>(b_obj);
    return a->find_subsequence(*b);
}

// --- map/where/reduce wrappers (templates) ---
template<typename T, typename MapF>
void* map_stack_new_t(void* obj, MapF f) {
    Stack<T>* s = static_cast<Stack<T>*>(obj);
    Stack<T> res = s->template map<MapF>(f);
    return static_cast<void*>( new Stack<T>(my_move(res)) );
}

template<typename T, typename Pred>
void* where_stack_new_t(void* obj, Pred p) {
    Stack<T>* s = static_cast<Stack<T>*>(obj);
    Stack<T> res = s->template where<Pred>(p);
    return static_cast<void*>( new Stack<T>(my_move(res)) );
}

template<typename T, typename Red>
Option<T> reduce_stack_t(void* obj, Red r) {
    Stack<T>* s = static_cast<Stack<T>*>(obj);
    return s->template reduce<Red>(r);
}

template<typename T, typename MapF>
void* map_deque_new_t(void* obj, MapF f) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    SegmentedDeque<T> res = d->template map<MapF>(f);
    return static_cast<void*>( new SegmentedDeque<T>(my_move(res)) );
}

template<typename T, typename Pred>
void* where_deque_new_t(void* obj, Pred p) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    SegmentedDeque<T> res = d->template where<Pred>(p);
    return static_cast<void*>( new SegmentedDeque<T>(my_move(res)) );
}

template<typename T, typename Red>
Option<T> reduce_deque_t(void* obj, Red r) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    return d->template reduce<Red>(r);
}

// --- sorting/merging helpers for deque ---
// insertion sort: requires operator< for T
template<typename T>
void* sort_deque_new(void* obj) {
    SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(obj);
    index_t n = d->size();
    if (n == 0) return static_cast<void*>( new SegmentedDeque<T>(16) );
    T* arr = new T[n];
    for (index_t i = 0; i < n; ++i) arr[i] = d->get(i);
    for (index_t i = 1; i < n; ++i) {
        T key = arr[i];
        index_t j = i;
        while (j > 0 && key < arr[j-1]) { arr[j] = arr[j-1]; --j; }
        arr[j] = key;
    }
    SegmentedDeque<T>* nd = new SegmentedDeque<T>(16);
    for (index_t i = 0; i < n; ++i) nd->push_back(arr[i]);
    delete [] arr;
    return static_cast<void*>(nd);
}

// merge: assume both sorted. Use only operator< (avoid <= operator)
template<typename T>
void* merge_deque_new(void* a_obj, void* b_obj) {
    SegmentedDeque<T>* a = static_cast<SegmentedDeque<T>*>(a_obj);
    SegmentedDeque<T>* b = static_cast<SegmentedDeque<T>*>(b_obj);
    SegmentedDeque<T>* nd = new SegmentedDeque<T>(16);
    index_t ia = 0, ib = 0;
    while (ia < a->size() && ib < b->size()) {
        // use only operator<: a <= b  <=>  !(b < a)
        if (!(b->get(ib) < a->get(ia))) { nd->push_back(a->get(ia)); ++ia; }
        else { nd->push_back(b->get(ib)); ++ib; }
    }
    while (ia < a->size()) { nd->push_back(a->get(ia)); ++ia; }
    while (ib < b->size()) { nd->push_back(b->get(ib)); ++ib; }
    return static_cast<void*>(nd);
}

// --- interactive_ops_t ---
// entry_void: pointer to ADTEntry (ui::ADTEntry*), store passed by ref, idx index into store
template<typename T>
void interactive_ops_t(void* entry_void, DynamicArray<ADTEntry>& store, index_t idx) {
    (void) idx; // may be unused in many instantiations

    ADTEntry* entry = static_cast<ADTEntry*>(entry_void);
    if (!entry) return;
    if (!entry->obj) { std::cout << "Empty entry\n"; return; }

    std::cout << "Operations for selected ADT (type " << static_cast<int>(entry->dt) << "):\n";
    std::cout << "1) Full print\n2) Elementary ops (push/pop/top or deque ops)\n3) Concat (replace)\n4) Subsequence (i j) (replace)\n5) Find subsequence\n6) Map/Where/Reduce (presets)\n7) Sort/Merge (deque only)\n0) Back\nChoice: ";
    int choice;
    while (!read_int_safe(choice)) std::cout << "Invalid: ";
    if (choice == 0) return;

    if (choice == 1) {
        if (entry->adt == ADTKind::STACK) print_stack_full_generic<T>(entry->obj);
        else print_deque_full_generic<T>(entry->obj);
        return;
    }

    if (choice == 2) {
        if (entry->adt == ADTKind::STACK) {
            std::cout << "Stack ops: 1) push 2) pop 3) top\nChoice: ";
            int c; while (!read_int_safe(c)) std::cout<<"Invalid: ";
            Stack<T>* s = static_cast<Stack<T>*>(entry->obj);
            if (c==1) { T v = read_one<T>(); s->push(v); std::cout << "Pushed.\n"; }
            else if (c==2) { s->pop(); std::cout << "Popped.\n"; }
            else { std::cout << "Top: "; print_one<T>(s->top()); std::cout << "\n"; }
        } else {
            std::cout << "Deque ops: 1) push_back 2) push_front 3) pop_back 4) pop_front 5) get(i)\nChoice: ";
            int c; while (!read_int_safe(c)) std::cout << "Invalid: ";
            SegmentedDeque<T>* d = static_cast<SegmentedDeque<T>*>(entry->obj);
            if (c==1) { T v = read_one<T>(); d->push_back(v); std::cout<<"push_back done\n"; }
            else if (c==2) { T v = read_one<T>(); d->push_front(v); std::cout<<"push_front done\n"; }
            else if (c==3) { d->pop_back(); std::cout<<"pop_back\n"; }
            else if (c==4) { d->pop_front(); std::cout<<"pop_front\n"; }
            else { index_t i; std::cout<<"Index: "; while(!read_index(i)) std::cout<<"Invalid index: "; print_one<T>(d->get(i)); std::cout<<"\n"; }
        }
        return;
    }

    if (choice == 3) {
        std::cout << "Enter index of other ADT to concat with: ";
        index_t j; while(!read_index(j) || j >= store.size()) { std::cout<<"Invalid index: "; return; }
        ADTEntry &other = store[j];
        if (!other.obj) { std::cout<<"Other empty\n"; return; }
        if (other.adt != entry->adt || other.dt != entry->dt) { std::cout<<"Type/ADT mismatch\n"; return; }
        void* newobj = nullptr;
        if (entry->adt == ADTKind::STACK) newobj = concat_stack_new<T>(entry->obj, other.obj);
        else newobj = concat_deque_new<T>(entry->obj, other.obj);
        if (entry->adt == ADTKind::STACK) delete_stack_t<T>(entry->obj);
        else delete_deque_t<T>(entry->obj);
        entry->obj = newobj;
        std::cout << "Concat applied and replaced current entry.\n";
        return;
    }

    if (choice == 4) {
        std::cout << "Enter i j (inclusive): ";
        index_t i, j; if (!read_index(i) || !read_index(j)) { std::cout << "Invalid\n"; return; }
        void* newobj = nullptr;
        if (entry->adt == ADTKind::STACK) newobj = subseq_stack_new<T>(entry->obj, i, j);
        else newobj = subseq_deque_new<T>(entry->obj, i, j);
        if (entry->adt == ADTKind::STACK) delete_stack_t<T>(entry->obj);
        else delete_deque_t<T>(entry->obj);
        entry->obj = newobj;
        std::cout << "Subsequence extracted and replaced current entry.\n";
        return;
    }

    if (choice == 5) {
        std::cout << "Enter index of subsequence ADT to search for: ";
        index_t j; if (!read_index(j)) { std::cout << "Invalid\n"; return; }
        if (j >= store.size()) { std::cout << "Invalid index\n"; return; }
        ADTEntry &sub = store[j];
        if (!sub.obj) { std::cout << "Other empty\n"; return; }
        if (sub.adt != entry->adt || sub.dt != entry->dt) { std::cout << "Type/ADT mismatch\n"; return; }
        Option<index_t> res;
        if (entry->adt == ADTKind::STACK) res = find_subseq_stack<T>(entry->obj, sub.obj);
        else res = find_subseq_deque<T>(entry->obj, sub.obj);
        if (res.has_value()) std::cout << "Found at: " << res.value() << "\n"; else std::cout << "Not found\n";
        return;
    }

    if (choice == 6) {
        std::cout << "Choose: 1) map  2) where  3) reduce\nChoice: ";
        int op; while (!read_int_safe(op)) std::cout << "Invalid: ";
        if (entry->adt == ADTKind::STACK) {
            // For brevity: UI-level presets expected to call these templates; real presets are implemented
            // in the higher-level UI code. Here we could call map/where/reduce templates if desired.
            std::cout << "Map/Where/Reduce presets handled in UI. Use the higher-level UI menu.\n";
        } else {
            std::cout << "Map/Where/Reduce for deque: use UI presets.\n";
        }
        return;
    }

    if (choice == 7) {
        if (entry->adt != ADTKind::DEQUE) { std::cout << "Sort/Merge only for deque\n"; return; }
        std::cout << "1) sort  2) merge\nChoice: ";
        int c; while(!read_int_safe(c)) std::cout<<"Invalid: ";
        if (c==1) {
            void* newd = sort_deque_new<T>(entry->obj);
            delete_deque_t<T>(entry->obj);
            entry->obj = newd;
            std::cout << "Sorted\n";
        } else {
            std::cout << "Enter other deque index to merge with: ";
            index_t j; while(!read_index(j) || j >= store.size()) { std::cout<<"Invalid index\n"; return; }
            ADTEntry &other = store[j];
            if (!other.obj || other.adt != ADTKind::DEQUE || other.dt != entry->dt) { std::cout<<"Other invalid\n"; return; }
            void* newd = merge_deque_new<T>(entry->obj, other.obj);
            delete_deque_t<T>(entry->obj);
            entry->obj = newd;
            std::cout << "Merged and replaced current entry.\n";
        }
        return;
    }

    std::cout << "Unknown choice\n";
}

} // namespace ui

#endif // UI_OPS_TEMPLATES_HPP
