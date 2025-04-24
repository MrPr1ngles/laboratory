#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "Option.hpp"
#include "Error.hpp"

template <typename T>
class LinkedList {
private:
    struct Node {
        T      value;
        Node*  next;
        Node(const T& v) : value(v), next(nullptr) {}
    };
    Node* head;
    int   len;

public:
    LinkedList() : head(nullptr), len(0) {}

    LinkedList(T* arr, int count) : head(nullptr), len(0) {
        for (int i = 0; i < count; ++i) {
            Append(arr[i]);
        }
    }

    LinkedList(const LinkedList<T>& other) : head(nullptr), len(0) {
        Node* curr = other.head;
        while (curr) {
            Append(curr->value);
            curr = curr->next;
        }
    }

    ~LinkedList() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    T GetFirst() const {
        if (len == 0) throw Errors[INDEX_OUT_OF_RANGE].message;
        return head->value;
    }

    T GetLast() const {
        if (len == 0) throw Errors[INDEX_OUT_OF_RANGE].message;
        Node* curr = head;
        while (curr->next) curr = curr->next;
        return curr->value;
    }

    T Get(int index) const {
        if (index < 0 || index >= len) throw Errors[INDEX_OUT_OF_RANGE].message;
        Node* curr = head;
        for (int i = 0; i < index; ++i) curr = curr->next;
        return curr->value;
    }

    LinkedList<T>* GetSubList(int start, int end) const {
        if (start < 0 || end >= len || start > end) throw Errors[INDEX_OUT_OF_RANGE].message;
        LinkedList<T>* sub = new LinkedList<T>();
        Node* curr = head;
        for (int i = 0; i < start; ++i) curr = curr->next;
        for (int i = start; i <= end; ++i) {
            sub->Append(curr->value);
            curr = curr->next;
        }
        return sub;
    }

    int GetLength() const {
        return len;
    }

    void Append(T item) {
        Node* n = new Node(item);
        if (!head) {
            head = n;
        } else {
            Node* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = n;
        }
        ++len;
    }

    void Prepend(T item) {
        Node* n = new Node(item);
        n->next = head;
        head = n;
        ++len;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > len) throw Errors[INDEX_OUT_OF_RANGE].message;
        if (index == 0) {
            Prepend(item);
            return;
        }
        Node* curr = head;
        for (int i = 0; i < index - 1; ++i) curr = curr->next;
        Node* n = new Node(item);
        n->next = curr->next;
        curr->next = n;
        ++len;
    }

    Option<T> TryGet(int index) const {
        if (index < 0 || index >= len) return Option<T>::None();
        return Option<T>::Some(Get(index));
    }

    Option<T> TryFirst() const {
        if (len == 0) return Option<T>::None();
        return Option<T>::Some(GetFirst());
    }

    Option<T> TryLast() const {
        if (len == 0) return Option<T>::None();
        return Option<T>::Some(GetLast());
    }
};

#endif 
