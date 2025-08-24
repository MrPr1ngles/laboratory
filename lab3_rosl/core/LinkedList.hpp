#ifndef CORE_LINKEDLIST_HPP
#define CORE_LINKEDLIST_HPP

// Простая однонаправленная реализация linked list,
// чтобы дать корректную реализацию ListSequence.

#include "Common.hpp"
#include "Error.hpp"

namespace core {

template<typename T>
class LinkedList {
public:
    struct Node {
        T value;
        Node* next;
        Node(const T& v): value(v), next(nullptr) {}
    };

private:
    Node* head_;
    Node* tail_;
    core::index_t size_;

public:
    LinkedList(): head_(nullptr), tail_(nullptr), size_(0) {}
    ~LinkedList() {
        Node* cur = head_;
        while (cur) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }

    core::index_t size() const { return size_; }

    T get(core::index_t idx) const {
        if (idx >= size_) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "LinkedList::get");
        Node* cur = head_;
        for (core::index_t i=0;i<idx;++i) cur = cur->next;
        return cur->value;
    }

    void append(const T& v) {
        Node* n = new Node(v);
        if (!head_) head_ = tail_ = n;
        else {
            tail_->next = n;
            tail_ = n;
        }
        ++size_;
    }

    void prepend(const T& v) {
        Node* n = new Node(v);
        n->next = head_;
        head_ = n;
        if (!tail_) tail_ = head_;
        ++size_;
    }

    void insert(core::index_t idx, const T& v) {
        if (idx > size_) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "LinkedList::insert");
        if (idx == 0) { prepend(v); return; }
        Node* cur = head_;
        for (core::index_t i=0;i<idx-1;++i) cur = cur->next;
        Node* n = new Node(v);
        n->next = cur->next;
        cur->next = n;
        if (!n->next) tail_ = n;
        ++size_;
    }

    void remove_at(core::index_t idx) {
        if (idx >= size_) core::abort_with_error(ErrorCode::INDEX_OUT_OF_RANGE, "LinkedList::remove_at");
        if (idx == 0) {
            Node* tmp = head_;
            head_ = head_->next;
            delete tmp;
            if (!head_) tail_ = nullptr;
        } else {
            Node* cur = head_;
            for (core::index_t i=0;i<idx-1;++i) cur = cur->next;
            Node* todel = cur->next;
            cur->next = todel->next;
            if (!cur->next) tail_ = cur;
            delete todel;
        }
        --size_;
    }
};

} // namespace core

#endif // CORE_LINKEDLIST_HPP
