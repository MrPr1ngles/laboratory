#ifndef FLOYD_HPP
#define FLOYD_HPP

#include "LinkedList.hpp"
#include <utility>

template<typename T>
std::pair<bool, typename LinkedList<T>::Node*> findCycleNode(const LinkedList<T>& lst){
    typename LinkedList<T>::Node* slow = lst.head;
    typename LinkedList<T>::Node* fast = lst.head;
    while(fast && fast->next){
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast) break;
    }
    if(!fast || !fast->next) return {false, nullptr};
    slow = lst.head;
    while(slow != fast){
        slow = slow->next;
        fast = fast->next;
    }
    return {true, slow};
}

template<typename T>
std::pair<bool, T> findCycle(const LinkedList<T>& lst){
    auto pr = findCycleNode(lst);
    if(!pr.first) return {false, T()};
    return {true, pr.second->value};
}

#endif 
