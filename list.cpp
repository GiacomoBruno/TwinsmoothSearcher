#include "list.h"

//actually push front
void LinkedList::push_front(void* n) {
    _size++;

    if(first == nullptr){
        first = new LLNode(n);
    }
    else
    {
        auto tmp = new LLNode(n);
        tmp->next = first;
        first = tmp;
    }
}

void LinkedList::remove_first() {
    if(first == nullptr) return;

    _size--;

    auto tmp = first;
    first = first->next;
    delete tmp;
}

void* LinkedList::pop()
{
    if(first == nullptr) return nullptr;
    auto res = first->value;
    _size--;
    auto tmp = first;
    first = first->next;
    delete tmp;
    return res;
}


LNode LinkedList::begin() const { return first; }

void LinkedList::clear() {
    while(first != nullptr)
    {
        auto tmp = first->next;
        delete first;
        first = tmp;
    }
}

bool LinkedList::empty() const { return _size == 0; }

void LinkedList::simple_merge(LinkedList *other) {
    auto other_start = other->begin();
    while(other_start != nullptr)
    {
        push_front(other_start->value);
        other_start = other_start->next;
    }
    other->clear();
    delete other;
}
