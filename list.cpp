#include "list.h"

void LinkedList::push_back(void* n) {
    _size++;

    if(first == nullptr || last == nullptr) {
        last =  new LLNode(n);
        first = last;
    }
    else
    {
        last->next = new LLNode(n);
        last = last->next;
    }
}

void LinkedList::remove_first() {
    if(first == nullptr) return;

    _size--;
    if(first == last)
    {
        delete first;
        first = nullptr;
        last = nullptr;
    }
    else
    {
        auto tmp = first;
        first = first->next;
        delete tmp;
    }
}

void* LinkedList::pop()
{
    if(first == nullptr) return nullptr;
    auto res = first->value;
    remove_first();
    return res;
}


LNode LinkedList::begin() { return first; }
LNode LinkedList::end(){ return last; }

void LinkedList::clear() {
    while(first != nullptr)
    {
        auto tmp = first->next;
        delete first;
        first = tmp;
    }
}

bool LinkedList::empty() const { return _size == 0; }
