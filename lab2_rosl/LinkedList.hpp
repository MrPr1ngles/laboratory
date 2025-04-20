// LinkedList.hpp
#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <stdexcept>
#include "Option.hpp"

template <typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node(const T& data) : data(data), next(nullptr) {}
    };
private:
    Node* head;
    int length;
public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>& other);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const;
    int GetLength() const;

    void Append(T item);
    void Prepend(T item);
    void InsertAt(T item, int index);
    LinkedList<T>* Concat(const LinkedList<T>* list) const;

    Option<T> TryGet(int index) const;
    Option<T> TryFirst() const;
    Option<T> TryLast() const;

    // Iterator
    class Iterator {
        Node* current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        Iterator& operator++() { if (current) current = current->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++*this; return tmp; }
        bool operator==(const Iterator& o) const { return current == o.current; }
        bool operator!=(const Iterator& o) const { return current != o.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end()   { return Iterator(nullptr); }
    const Iterator begin() const { return Iterator(head); }
    const Iterator end()   const { return Iterator(nullptr); }
};

template <typename T>
LinkedList<T>::LinkedList() : head(nullptr), length(0) {}

template <typename T>
LinkedList<T>::LinkedList(T* items, int count) : head(nullptr), length(0) {
    for (int i = 0; i < count; i++)
        Append(items[i]);
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), length(0) {
    for (Node* cur = other.head; cur; cur = cur->next)
        Append(cur->data);
}

template <typename T>
LinkedList<T>::~LinkedList() {
    while (head) {
        Node* nxt = head->next;
        delete head;
        head = nxt;
    }
}

template <typename T>
T LinkedList<T>::GetFirst() const {
    if (!head) throw std::out_of_range("Index out of range");
    return head->data;
}

template <typename T>
T LinkedList<T>::GetLast() const {
    if (!head) throw std::out_of_range("Index out of range");
    Node* cur = head;
    while (cur->next) cur = cur->next;
    return cur->data;
}

template <typename T>
T LinkedList<T>::Get(int index) const {
    if (index < 0 || index >= length) throw std::out_of_range("Index out of range");
    Node* cur = head;
    for (int i = 0; i < index; i++) cur = cur->next;
    return cur->data;
}

template <typename T>
LinkedList<T>* LinkedList<T>::GetSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
        throw std::out_of_range("Index out of range");
    LinkedList<T>* sub = new LinkedList<T>();
    Node* cur = head;
    for (int i = 0; i < startIndex; i++) cur = cur->next;
    for (int i = startIndex; i <= endIndex; i++) {
        sub->Append(cur->data);
        cur = cur->next;
    }
    return sub;
}

template <typename T>
int LinkedList<T>::GetLength() const { return length; }

template <typename T>
void LinkedList<T>::Append(T item) {
    Node* node = new Node(item);
    if (!head) head = node;
    else {
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }
    length++;
}

template <typename T>
void LinkedList<T>::Prepend(T item) {
    Node* node = new Node(item);
    node->next = head;
    head = node;
    length++;
}

template <typename T>
void LinkedList<T>::InsertAt(T item, int index) {
    if (index < 0 || index > length) throw std::out_of_range("Index out of range");
    if (index == 0) { Prepend(item); return; }
    Node* cur = head;
    for (int i = 0; i < index - 1; i++) cur = cur->next;
    Node* node = new Node(item);
    node->next = cur->next;
    cur->next = node;
    length++;
}

template <typename T>
LinkedList<T>* LinkedList<T>::Concat(const LinkedList<T>* list) const {
    auto* res = new LinkedList<T>(*this);
    for (Node* cur = list->head; cur; cur = cur->next)
        res->Append(cur->data);
    return res;
}

template <typename T>
Option<T> LinkedList<T>::TryGet(int index) const {
    if (index < 0 || index >= length) return Option<T>::None();
    return Option<T>::Some(Get(index));
}
template <typename T>
Option<T> LinkedList<T>::TryFirst() const {
    if (!head) return Option<T>::None();
    return Option<T>::Some(head->data);
}
template <typename T>
Option<T> LinkedList<T>::TryLast() const {
    if (!head) return Option<T>::None();
    Node* cur = head; while (cur->next) cur = cur->next;
    return Option<T>::Some(cur->data);
}

#endif // LINKEDLIST_HPP
